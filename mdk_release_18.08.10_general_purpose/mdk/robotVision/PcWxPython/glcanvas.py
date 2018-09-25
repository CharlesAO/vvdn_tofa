import wx
import sys
import cv2
from wx import glcanvas
from OpenGL.GL import *
from OpenGL.GLU import *
import ctypes

class CanvasBase(glcanvas.GLCanvas):

    def __init__(self, parent):

        attribs=[glcanvas.WX_GL_RGBA, glcanvas.WX_GL_DOUBLEBUFFER, glcanvas.WX_GL_DEPTH_SIZE,24];  
        super(CanvasBase,self).__init__(parent, attribList=attribs)
        self.context = glcanvas.GLContext(self)
        self.init = False
        self.dist = 5.0
        self.anglex = 0.0
        self.angley = 0.0
        self.size = None
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        wx.CallLater(500, self.OnShow) #Find a better way to wait
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_MOUSEWHEEL, self.OnZoom)
        self.dragging = False
        self.Bind(wx.EVT_LEFT_DOWN, self.OnMouseDown)
        self.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        self.Bind(wx.EVT_LEFT_UP, self.OnMouseUp)

    def OnEraseBackground(self, event):
        pass # Do nothing, to avoid flashing on MSW.

    def OnShow(self):
        self.Bind(wx.EVT_SIZE, self.OnSize)

    def OnSize(self, event):
        size = self.size = self.GetClientSize()
        if self.GetParent().IsShown():
            if glcanvas.GLContext(self):
                self.SetCurrent(self.context)
        glViewport(0, 0, size.width, size.height)
        self.OnReshape(size.width, size.height)
        if event:
            event.Skip()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.SetCurrent(self.context)
        if not self.init:
            self.InitGL()
            self.init = True
        self.OnDraw()

    def OnReshape(self, width, height):
        """Reshape the OpenGL viewport based on the dimensions of the window."""

        glMatrixMode(GL_PROJECTION)
        glLoadIdentity();
        gluPerspective(69.0, float(width)/float(height), 0.0001, 500.0);
        
        # glMatrixMode(GL_PROJECTION); #Select The Projection Matrix
        # glLoadIdentity();            # Reset The Projection Matrix
        # fx = 1747.720082167543/2.0
        # fy = 1751.352387112096/2.0
        # w = self.real_width.0
        # h = self.real_height.0
        # cx = 320.0
        # cy = 200.0
        # far = 500.0
        # near = 0.0001

        # perspMatrix=(
        #   2*fx/w,     0,           0,                      0,
        #   0,          2*fy/h,      0,                      0,
        #   2*(cx/w)-1, 2*(cy/h)-1, -(far+near)/(far-near), -1,
        #   0,          0,          -2*far*near/(far-near),  0
        # )

        # perspMatrix=(
        #     2*fx/w,        0,                      0,                        0,
        #          0,   2*fy/h,                      0,                        0,
        #          0,        0, (-far-near)/(far-near), -2*far*near/(far - near),
        #          0,        0,                     -1,                        0
        # )
        # glMultMatrixd(perspMatrix);

        glMatrixMode(GL_MODELVIEW)

    def OnZoom(self, event):
        delta = self.dist/20;
        self.dist += -delta if event.GetWheelRotation()>0 else delta
        self.Refresh()

    def OnMouseDown(self, event):
        self.down_position = event.GetPosition()
        self.dragging = True

    def OnMouseMotion(self, event):
        if self.dragging:
            move_position = event.GetPosition()
            self.anglex += (self.down_position[0]-move_position[0])/4.0
            self.angley -= (self.down_position[1]-move_position[1])/4.0
            self.down_position = move_position
        self.Refresh()

    def OnMouseUp(self, event):
        up_position = event.GetPosition()
        self.dragging = False
        

class DispCanvas(CanvasBase):

    def InitGL(self):
        glEnable(GL_ALPHA_TEST)
        glAlphaFunc(GL_EQUAL, 1.0)
        glEnable(GL_DEPTH_TEST)
        glCullFace(GL_BACK)

        self.vertices = (ctypes.c_float * (1280*720*3))()
        self.colors = (ctypes.c_float * (1280*720*4))()
        self.indices = (ctypes.c_int * ((1280*720) + (1280-1)*(720-2)))()
        #self.indeces_len = (self.real_width*self.real_height) + (self.real_width-1)*(self.real_height-2) -1
        self._gva = ctypes.CDLL('./libgva.so')
        self._gva.setup_mapping(ctypes.c_int(1280), ctypes.c_int(720))
        self.OnSize(None)

    def OnDraw(self):
        indeces_len = int(self._gva.get_vertex_array(ctypes.byref(self.vertices),ctypes.byref(self.colors),ctypes.byref(self.indices)))

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        glLoadIdentity();
        gluLookAt(0.0,0.0,-self.dist, 0.0,0.0,0.0, 0.0,-1.0,0.0)
        glRotatef(self.anglex, 0.0, 1.0, 0.0);
        glRotatef(self.angley, 1.0, 0.0, 0.0);

        glTranslatef(0.0,0.0,-2.5)

        glEnableClientState( GL_VERTEX_ARRAY )
        glEnableClientState( GL_COLOR_ARRAY )
        glVertexPointer( 3, GL_FLOAT, 0, self.vertices )
        glColorPointer( 4, GL_FLOAT, 0, self.colors )
        glDrawElements( GL_TRIANGLE_STRIP, indeces_len, GL_UNSIGNED_INT, self.indices )
        glDisableClientState( GL_VERTEX_ARRAY )
        glDisableClientState( GL_COLOR_ARRAY )

        self.SwapBuffers()
        self.Refresh()
