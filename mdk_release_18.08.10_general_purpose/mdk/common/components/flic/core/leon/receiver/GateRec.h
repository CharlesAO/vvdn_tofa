///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Gate (SReceiver/MSender pair)
///

#ifndef __REC_GATE_H__
#define __REC_GATE_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>

//fwd decl
template<typename T> class SGateRec;

//#########################################################
template <typename T> class PlgGate : public IPlugin
{
  public:
    bool en;
    SGateRec<T> in;
    MSender <T> out;
    PlgGate();
};

//#########################################################
template<typename T> class SGateRec: public ISReceiver<T>
{
  public:
      void Destroy (){};
      void Delete  (){};
      PlgGate<T>   *ref;

      using ISReceiver<T>::Create;

   //Slave behaviour (Sender is allowed to Post any time)
      int  Post   (Io *w, T *m, TimeSpec *ts, IBuffCmd<T> *c = NULL)
      {
          UNUSED(w);UNUSED(c);
          int sc = OK;
          if(ref->en)
             sc = ref->out.Send(m, ts);
          //else, 'm' gets discarded
          return sc;
      };

      void SetLog (IBuffLog<T> *){};

  private:
      void Create(uint32_t, IBuffCmd<T> *, ISync *) {}
      int  Receive   (T *,  IBuffCmd<T> * = NULL) {return 0;};
      int  TryReceive(T *,  IBuffCmd<T> * = NULL) {return 0;};
};

//#########################################################
template <typename T> PlgGate<T>::PlgGate()
{
    en     = true;
    in.ref = this;
    Add(&in, ".in");
    Add(&out,".out");
}

#endif
