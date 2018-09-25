import wx
import wx.lib.scrolledpanel as scrolled
import cv2
import os
import sys
import numpy as np
import time 
import threading
from threading import Thread
import mmap
import struct
import subprocess
from glcanvas import *

thread_list = []
thread_vTrackStereo = []


calibration_file_path = ""
resolution = "VGA"
connection_status = 0
algo = [True, True]

if os.path.exists("/tmp/mem_confidence"):
    os.remove("/tmp/mem_confidence")
if os.path.exists("/tmp/mem_mapped_disp"):
    os.remove("/tmp/mem_mapped_disp")
if os.path.exists("/tmp/mem_mapped_graph"):
    os.remove("/tmp/mem_mapped_graph")
if os.path.exists("/tmp/mem_mapped_left"):
    os.remove("/tmp/mem_mapped_left")
if os.path.exists("/tmp/mem_mapped_rdisp"):
    os.remove("/tmp/mem_mapped_rdisp")
if os.path.exists("/tmp/mem_mapped_right"):
    os.remove("/tmp/mem_mapped_right")
if os.path.exists("/tmp/mem_mapped_status"):
    os.remove("/tmp/mem_mapped_status")
if os.path.exists("/tmp/mem_mapped_vtrack"):
    os.remove("/tmp/mem_mapped_vtrack")

subprocess.Popen(["gcc","-fPIC","-shared","-o","libgva.so","get_vertex_array.c","-O3"]).wait()
os.environ["GDK_BACKEND"] = "x11" #Solves Wayland bug: https://github.com/wxWidgets/Phoenix/issues/915

os.system("cd vTrack_Stereo; mkdir build; cd build; cmake ..; make")
subprocess.Popen("killall vTrackStereo", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait()


def run_vtrackstereo(statusbar):
    global resolution, calibration_file_path, connection_status, algo
    connection_status = 1
    command_line = ["./vTrack_Stereo/build/vTrackStereo","--device","myriad","--vis","detailed","--res",resolution]
    if calibration_file_path != "":
        command_line.extend(["-h",calibration_file_path])

    if algo[0] != algo[1]:
        command_line.extend(["--alg","stereo" if algo[0] else "vtrack"])

    print(">> "+" ".join(command_line))
    proc = subprocess.Popen(command_line)
    mem_mapped_p = "/tmp/mem_mapped_status"
    t = threading.currentThread()
    while not os.path.exists(mem_mapped_p) or os.stat(mem_mapped_p).st_size == 0:
        if not getattr(t, "do_run", True):
            return
        time.sleep(0.01);

    mem_mapped = open(mem_mapped_p,'r')
    mml = mmap.mmap(mem_mapped.fileno(), 0, prot=mmap.PROT_READ)
    while getattr(t, "do_run", True):
        status1 = struct.unpack("I", bytearray(mml.read(4)))[0]
        mml.seek(0)
        time.sleep(5);
        status2 = struct.unpack("I", bytearray(mml.read(4)))[0]
        mml.seek(0)
        if status1 == status2:
            break
        else:
            wx.CallAfter(statusbar.SetStatusText,"Status: Connected.")
            connection_status = 2
    wx.CallAfter(statusbar.SetStatusText,"Status: Not connected.")
    proc.kill()
    connection_status = 0


def redraw_panels(panel, channel_n):
    mem_mapped_p = "/tmp/mem_mapped_"+panel.GetName()
    t = threading.currentThread()
    while not os.path.exists(mem_mapped_p) or os.stat(mem_mapped_p).st_size == 0:
        if not getattr(t, "do_run", True):
            return
        time.sleep(1)
    mem_mapped = open(mem_mapped_p,'r')
    mml = mmap.mmap(mem_mapped.fileno(), 0, prot=mmap.PROT_READ)
    while getattr(t, "do_run", True):
        read_width = struct.unpack("I", bytearray(mml.read(4)))[0]
        read_height = struct.unpack("I", bytearray(mml.read(4)))[0]
        
        if os.stat(mem_mapped_p).st_size > read_width*read_height*channel_n:
            fps = struct.unpack("f", bytearray(mml.read(4)))[0]
            if hasattr(panel, 'infos'):
                wx.CallAfter(panel.infos.SetLabel, "{}x{} @ {:.3f} fps".format(read_width, read_height, fps))
            frame = np.fromstring(mml.read(read_width*read_height*channel_n), dtype=np.uint8)
            try:
                frame = frame.reshape((read_height,read_width,channel_n))
                if channel_n == 1:
                    frame = np.tile(np.expand_dims(frame, axis=2),3)
                panel.bmp = wx.Bitmap.FromBuffer(read_width,read_height,frame)
            except ValueError:
                mml = mmap.mmap(mem_mapped.fileno(), 0, prot=mmap.PROT_READ)
            #wx.CallAfter(panel.bmp.CopyFromBuffer,frame)
        wx.CallAfter(panel.Refresh)
        mml.seek(0)
        time.sleep(0.0030)

def redraw_graph(panel, channel_n):
    mem_mapped = "/tmp/mem_mapped_"+panel.GetName()
    t = threading.currentThread()
    while not os.path.exists(mem_mapped) or os.stat(mem_mapped).st_size < 101:
        if not getattr(t, "do_run", True):
            return
        time.sleep(1)
    mem_mapped = open(mem_mapped,'r+')
    mml = mmap.mmap(mem_mapped.fileno(), 0)

    while getattr(t, "do_run", True):
        mml.write(getattr(t, "graph_name", True)+"\0")
        mml.seek(100, 0)
        frame = np.fromstring(mml.read(640*480*channel_n), dtype=np.uint8)
        frame = frame.reshape((480,640,channel_n))
        if channel_n == 1:
            frame = np.tile(np.expand_dims(frame, axis=2),3)
        wx.CallAfter(panel.bmp.CopyFromBuffer,frame)
        wx.CallAfter(panel.Refresh)
        mml.seek(0)
        time.sleep(0.030)

def OnPaint(event):
    panel = event.GetEventObject()
    parent = panel.GetParent()
    dc = wx.BufferedPaintDC(panel)
    if panel.bmp.IsOk():
        img = panel.bmp.ConvertToImage()
        ratio_i = (float(img.GetSize()[0])/float(img.GetSize()[1]))
        ratio_p = (float(parent.GetSize()[0])/float(parent.GetSize()[1]))

        if(ratio_p > ratio_i):
            new_height = parent.GetSize()[1]
            new_width = int(ratio_i*new_height)
        else:
            new_width = parent.GetSize()[0]
            new_height = int(new_width/ratio_i)

        panel.SetMinSize((new_width,new_height))
        panel.SetMaxSize((new_width,new_height))
        
        dc.DrawBitmap(img.Scale(new_width, new_height, wx.IMAGE_QUALITY_HIGH).ConvertToBitmap(), 0, 0)
    parent.Layout()

class vTS_GUI(wx.App):

    class StereoTab(wx.Panel):
        def __init__(self, parent):
            wx.Panel.__init__(self, parent)
            hbox = wx.BoxSizer(wx.HORIZONTAL)
            self.SetSizer(hbox)

            result_panel = wx.Panel(self, style=wx.BORDER_SUNKEN)
            hbox.Add(result_panel, 1, wx.EXPAND)
            rp_rwp_vbox = wx.BoxSizer(wx.VERTICAL)
            result_panel.SetSizer(rp_rwp_vbox)

            disp_panel_scroll = wx.Panel(result_panel)
            disp_panel_scroll.SetBackgroundColour((0, 0, 0))
            sbox = wx.BoxSizer(wx.VERTICAL)
            isbox = wx.BoxSizer(wx.HORIZONTAL)
            disp_panel_scroll.SetSizer(sbox)
            disp_panel = wx.Panel(disp_panel_scroll, name="disp")
            sbox.Add(isbox, 1, wx.ALIGN_CENTER)
            isbox.Add(disp_panel, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.ALL, 10)
            rp_rwp_vbox.Add(disp_panel_scroll, 1, wx.EXPAND)

            internal_sizer = wx.BoxSizer(wx.VERTICAL)
            disp_panel.SetSizer(internal_sizer)

            disp_panel.infos = wx.StaticText(disp_panel, -1, "0.0 fps") 

            sld = wx.Slider(disp_panel, value = 255, minValue = 0, maxValue = 255,
                     style = wx.SL_HORIZONTAL|wx.SL_LABELS)
            sld.Bind(wx.EVT_SLIDER, self.OnSliderScroll)

            slider_map = open('/tmp/mem_confidence','wb+')
            slider_map.write(struct.pack("=B",255))
            slider_map.flush()
            self.mml = mmap.mmap(slider_map.fileno(), 1)

            top_internal_sizer = wx.BoxSizer(wx.VERTICAL)
            top_internal_sizer.Add(disp_panel.infos, 1, wx.ALL | wx.ALIGN_RIGHT, border=5)
            bottom_internal_sizer = wx.BoxSizer(wx.HORIZONTAL)
            bottom_internal_sizer.Add(sld, 1, wx.ALL | wx.ALIGN_BOTTOM, border=5)
            
            internal_sizer.Add(top_internal_sizer, 1, wx.EXPAND | wx.ALL, border=5)            
            internal_sizer.Add(bottom_internal_sizer, 1,  wx.EXPAND | wx.ALL, border=5)
            disp_panel.bmp = wx.Bitmap()

            frame = np.zeros([400, 640, 3])
            disp_panel.bmp = wx.Bitmap.FromBuffer(640, 400, frame)

            disp_panel.Bind(wx.EVT_PAINT, OnPaint)
            t = Thread(target=redraw_panels, args=(disp_panel,3))
            t.do_run = True
            t.start()
            thread_list.append(t)
    
        def OnSliderScroll(self, event):
            self.mml.seek(0)
            self.mml.write(struct.pack("=B",event.GetEventObject().GetValue()))
            self.mml.seek(0)


    class VTrackTab(wx.Panel):

        class Graph(wx.Frame):
            def __init__(self, parent):
                wx.Frame.__init__(self, parent, title='InteToolKit')
                vbox = wx.BoxSizer(wx.VERTICAL)
                hbox = wx.BoxSizer(wx.HORIZONTAL)
                self.SetSizer(vbox)

                graph_list = ["Latency","Inliers Histogram","Live Feature count","Coverage","Nr Of Features Histogram",
                              "Live Coverage", "Live Feature count", "Tracking length histogram","Current age histogram"]

                ch = wx.Choice(self,-1,choices=graph_list,size=(200,-1),pos=(15,15))
                ch.SetSelection(0)
                ch.Bind(wx.EVT_CHOICE, self.OnChoice)
                vbox.Add(ch, 1, wx.EXPAND)
                
                graph_panel = wx.Panel(self, name="graph")
                vbox.Add(hbox, 1, wx.EXPAND)
                hbox.Add(graph_panel, 1, wx.EXPAND | wx.ALL, border=10)
                graph_panel.SetBackgroundColour((0, 0, 0))
                self.SetMinSize((640,480))
                self.SetMaxSize((640,480))

                self.img = wx.Image(640, 480)
                self.img = wx.Image(640,480)
                graph_panel.bmp = wx.Bitmap(self.img)

                graph_panel.Bind(wx.EVT_PAINT, OnPaint)
                self.thrd = Thread(target=redraw_graph, args=(graph_panel,3))
                self.thrd.graph_name = "Latency"
                self.thrd.do_run = True
                self.thrd.start()
                thread_list.append(self.thrd)

                self.Bind(wx.EVT_CLOSE, self.OnQuit)

            def OnQuit(self, event):
                self.Hide()

            def OnChoice(self, event):
                graph_title = event.GetEventObject().GetStringSelection()
                self.thrd.graph_name = graph_title

        def __init__(self, parent):
            wx.Panel.__init__(self, parent)
            hbox = wx.BoxSizer(wx.HORIZONTAL)
            self.SetSizer(hbox)

            result_panel = wx.Panel(self, style=wx.BORDER_SUNKEN)
            hbox.Add(result_panel, 1, wx.EXPAND)
            rp_rwp_vbox = wx.BoxSizer(wx.VERTICAL)
            result_panel.SetSizer(rp_rwp_vbox)

            vtrack_panel_scroll = wx.Panel(result_panel)
            vtrack_panel_scroll.SetBackgroundColour((0, 0, 0))
            sbox = wx.BoxSizer(wx.VERTICAL)
            isbox = wx.BoxSizer(wx.HORIZONTAL)
            vtrack_panel_scroll.SetSizer(sbox)
            vtrack_panel = wx.Panel(vtrack_panel_scroll, name="vtrack")
            sbox.Add(isbox, 1, wx.ALIGN_CENTER)
            isbox.Add(vtrack_panel, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.ALL, 10)
            rp_rwp_vbox.Add(vtrack_panel_scroll, 1, wx.EXPAND)

            internal_sizer = wx.BoxSizer(wx.HORIZONTAL)
            vtrack_panel.SetSizer(internal_sizer)

            vtrack_panel.infos = wx.StaticText(vtrack_panel, -1, "0.0 fps") 

            btn = wx.Button(vtrack_panel, -1, "Show Stats")
            btn.Bind(wx.EVT_BUTTON,self.OnClicked) 
            left_internal_sizer = wx.BoxSizer(wx.VERTICAL)
            left_internal_sizer.Add(btn, 0, wx.ALL, border=5)
            right_internal_sizer = wx.BoxSizer(wx.VERTICAL)
            right_internal_sizer.Add(vtrack_panel.infos, 0, wx.ALL | wx.ALIGN_RIGHT, border=5)
            
            internal_sizer.Add(left_internal_sizer, 1, wx.ALL, border=5)
            internal_sizer.Add(right_internal_sizer, 1, wx.ALL, border=5)
            
            vtrack_panel.bmp = wx.Bitmap()
            frame = np.zeros([400, 640, 3])
            vtrack_panel.bmp = wx.Bitmap.FromBuffer(640, 400, frame)

            vtrack_panel.Bind(wx.EVT_PAINT, OnPaint)
            t = Thread(target=redraw_panels, args=(vtrack_panel,3))
            t.do_run = True
            t.start()
            thread_list.append(t)

            self.graph = self.Graph(self)

        def OnClicked(self, event):
            self.graph.Show()

    class AlgoTab(wx.Panel):
        def __init__(self, parent):
            self.new_size = (1,1)
            wx.Panel.__init__(self, parent,  style=wx.BORDER_SUNKEN)
            
            dbox = wx.BoxSizer(wx.VERTICAL)
            self.SetSizer(dbox)
            border_panel = wx.Panel(self)
            dbox.Add(border_panel, 1, wx.EXPAND | wx.ALL, border=10)

            hbox = wx.BoxSizer(wx.VERTICAL)
            splitter = wx.SplitterWindow(border_panel)

            border_panel.SetSizer(hbox)
            top_panel = wx.Panel(splitter, style=wx.BORDER_SUNKEN)
            bottom_panel = wx.Panel(splitter, style=wx.BORDER_SUNKEN)

            splitter.SplitHorizontally(bottom_panel, top_panel,0)
            self.proportion = 0.7
            splitter.SetMinimumPaneSize(100)
            hbox.Add(splitter, 1, wx.EXPAND)

            #Top Panel (lp)---------------------------
            lp_vbox = wx.BoxSizer(wx.VERTICAL)
            top_panel.SetSizer(lp_vbox)
            cameras_grid = wx.GridSizer(1, 2, 10, 10)
            lp_vbox.Add(cameras_grid, 1, wx.EXPAND)

            left_panel_scroll = wx.Panel(top_panel, style=wx.BORDER_SUNKEN)
            left_panel_scroll.SetBackgroundColour((0, 0, 0))
            sbox = wx.BoxSizer(wx.VERTICAL)
            isbox = wx.BoxSizer(wx.HORIZONTAL)
            left_panel_scroll.SetSizer(sbox)
            
            left_view = wx.Panel(left_panel_scroll, name="left")
            sbox.Add(isbox, 1,  wx.ALIGN_CENTER)
            isbox.Add(left_view, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.ALL, 10)

            right_panel_scroll = wx.Panel(top_panel, style=wx.BORDER_SUNKEN)
            right_panel_scroll.SetBackgroundColour((0, 0, 0))
            sbox = wx.BoxSizer(wx.VERTICAL)
            isbox = wx.BoxSizer(wx.HORIZONTAL)
            right_panel_scroll.SetSizer(sbox)
            right_view = wx.Panel(right_panel_scroll, name="right")
            sbox.Add(isbox, 1, wx.ALIGN_CENTER)
            isbox.Add(right_view, 1, wx.EXPAND | wx.ALIGN_CENTER_VERTICAL | wx.ALL, 10)

            cameras_grid.AddMany([(left_panel_scroll, 1, wx.EXPAND), (right_panel_scroll, 1, wx.EXPAND)])
            #Bottom Panel (rp)--------------------------
            rp_vbox = wx.BoxSizer(wx.HORIZONTAL)
            bottom_panel.SetSizer(rp_vbox)

            left_view.bmp = wx.Bitmap()
            frame = np.zeros([400, 640, 3])
            left_view.bmp = wx.Bitmap.FromBuffer(640, 400, frame)

            right_view.bmp = wx.Bitmap()
            frame = np.zeros([400, 640, 3])
            right_view.bmp = wx.Bitmap.FromBuffer(640, 400, frame)

            left_view.Bind(wx.EVT_PAINT, OnPaint)
            right_view.Bind(wx.EVT_PAINT, OnPaint)
            t = Thread(target=redraw_panels, args=(left_view,1))
            t.do_run = True
            t.start()
            thread_list.append(t)
            t = Thread(target=redraw_panels, args=(right_view,1))
            t.do_run = True
            t.start()
            thread_list.append(t)
            
            notebook = wx.Notebook(bottom_panel)
            # Create the tabs
            tab1 = vTS_GUI.StereoTab(notebook)
            tab2 = vTS_GUI.VTrackTab(notebook)
            tab3 = vTS_GUI.ThreeDTab(notebook)
            notebook.AddPage(tab1, "Stereo Vision")
            notebook.AddPage(tab2, "vTrack")
            notebook.AddPage(tab3, "3D Visualizer")
            rp_vbox.Add(notebook, 1, wx.EXPAND)

            splitter.Bind(wx.EVT_SIZE, self.OnSize)
            splitter.Bind(wx.EVT_SPLITTER_SASH_POS_CHANGED, self.OnSashChange)

        def OnSashChange(self, event):
            self.proportion = float(event.GetEventObject().GetSashPosition()) / float(event.GetEventObject().GetSize()[1])

        def OnSize(self, event):
            event.GetEventObject().SetSashPosition(event.GetEventObject().GetSize()[1]*self.proportion)
            event.Skip()

    class ThreeDTab(wx.Panel):
        def __init__(self, parent):
            wx.Panel.__init__(self, parent,  style=wx.BORDER_SUNKEN)
            hbox = wx.BoxSizer(wx.VERTICAL)
            self.SetSizer(hbox)
            self.SetBackgroundColour((0, 0, 0))

            canvas = DispCanvas(self)
            hbox.Add(canvas, 1, wx.EXPAND)
            self.Layout()
        
        def OnZoom(self, event):
            pass

    def __init__(self):
        wx.App.__init__(self)
        self.frame = wx.Frame(None, title='Intel-Movidius Robot Vision ToolKit')
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        self.frame.SetSizer(hbox)

        panel = vTS_GUI.AlgoTab(self.frame)
        hbox.Add(panel, 1, wx.EXPAND)

        menuBar = wx.MenuBar()
        
        fileMenu = wx.Menu()
        exitMenuItem = fileMenu.Append(wx.NewId(), "Exit", "Exit the application")
        menuBar.Append(fileMenu, "&File")
        self.Bind(wx.EVT_MENU, self.OnQuit, exitMenuItem)

        connectMenu = wx.Menu()

        openCalibMenuItem = connectMenu.Append(wx.NewId(), "Calib", "Choose calibration file")


        algoMenu = wx.Menu()
        stereoitem = algoMenu.Append(wx.NewId(),"Stereo",
                                      "Enable Stereo functionality",
                                      wx.ITEM_CHECK)
        stereoitem.Check()
        vTrackitem = algoMenu.Append(wx.NewId(), "vTrack",
                                   "Enable vTrack functionality",
                                   wx.ITEM_CHECK)
        vTrackitem.Check()

        connectMenu.Append(wx.ID_ANY, 'Algorithm',algoMenu)


        resolutionMenu = wx.Menu()
        vgaitem = resolutionMenu.Append(wx.NewId(), "VGA",
                                   "Small Resolution",
                                   wx.ITEM_RADIO)
        hditem = resolutionMenu.Append(wx.NewId(),"720p",
                                      "Big Resolution",
                                      wx.ITEM_RADIO)

        connectMenu.Append(wx.ID_ANY, 'Resolution',resolutionMenu)

        connectMenuItem = connectMenu.Append(wx.NewId(), "Connect", "Connect the application")
        disconnectMenuItem = connectMenu.Append(wx.NewId(), "Disconnect", "Disconnect the application")
        menuBar.Append(connectMenu, "&Connection")
        self.Bind(wx.EVT_MENU, self.OnSetResolutionVGA, vgaitem)
        self.Bind(wx.EVT_MENU, self.OnSetResolution720P, hditem)
        self.Bind(wx.EVT_MENU, self.OnSetAlgoStereo, stereoitem)
        self.Bind(wx.EVT_MENU, self.OnSetAlgovTrack, vTrackitem)
        self.Bind(wx.EVT_MENU, self.OnOpenCalib, openCalibMenuItem)
        self.Bind(wx.EVT_MENU, self.ConnectBoard, connectMenuItem)
        self.Bind(wx.EVT_MENU, self.DisconnectBoard, disconnectMenuItem)
        
        self.frame.SetMenuBar(menuBar)

        self.statusbar = self.frame.CreateStatusBar(1)
        self.statusbar.SetStatusText('The program is not connected to the board')
        global connection_status
        connection_status = 0

        icon = wx.Icon()
        icon.CopyFromBitmap(wx.Bitmap((os.path.dirname(sys.argv[0]) or ".")+"/vTrack_Stereo/icon.jpg", wx.BITMAP_TYPE_ANY))
        self.frame.SetIcon(icon)

        self.frame.SetMinSize((1150,980))
        self.frame.SetSize((1150,980))
        self.frame.Centre()
        self.frame.Bind(wx.EVT_CLOSE, self.OnQuit)
        self.frame.Show(True)

    def OnSetAlgoStereo(self, event):
        global algo
        algo[0] = event.IsChecked()
        
    def OnSetAlgovTrack(self, event):
        global algo
        algo[1] = event.IsChecked()

    def OnSetResolutionVGA(self, event):
        global resolution
        resolution = "VGA"

    def OnSetResolution720P(self, event):
        global resolution
        resolution = "720P"

    def OnOpenCalib(self, e): 
        dlg = wx.FileDialog(None, "Choose a Calibration file", os.getcwd(), style=wx.FD_OPEN | wx.STAY_ON_TOP | wx.CENTRE)
        if dlg.ShowModal() == wx.ID_OK: 
            global calibration_file_path
            calibration_file_path = dlg.GetPath()
            dlg.Destroy()

    def ConnectBoard(self, event):
        global connection_status
        self.statusbar.SetStatusText('Connecting...')
        if connection_status==0:
            t = Thread(target=run_vtrackstereo, args=(self.statusbar,))
            t.do_run = True
            t.start()
            thread_vTrackStereo.append(t)

    def DisconnectBoard(self, event):
        global connection_status, thread_vTrackStereo
        if connection_status==2:
            subprocess.Popen("killall vTrackStereo", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait()
            for t in thread_vTrackStereo:
                t.do_run = False
            thread_vTrackStereo = []
            connection_status=0

    def OnQuit(self, event):
        subprocess.Popen("killall vTrackStereo", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait()
        for t in thread_list:
            t.do_run = False
        for t in thread_vTrackStereo:
            t.do_run = False
        self.frame.Destroy()

if __name__ == '__main__':
    app = vTS_GUI()
    app.MainLoop()
