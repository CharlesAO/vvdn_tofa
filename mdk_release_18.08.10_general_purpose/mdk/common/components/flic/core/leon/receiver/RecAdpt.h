///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Adapter (SReceiver/MSender pair)
///

#ifndef __REC_ADPT_H__
#define __REC_ADPT_H__

#include <Flic.h>

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>

//fwd decl
template<typename TI, typename TO> class SRecAdapt;


//#########################################################
template <typename TI, typename TO> class PlgAdapter : public IPlugin
{
  public:
    SRecAdapt<TI, TO> in;
    MSender  <TO>     out;
    PlgAdapter();
    virtual void Convert(const TI *dIn, TO *dOut) = 0;
};

//#########################################################
template<typename TI, typename TO> class SRecAdapt: public ISReceiver<TI>
{
  public:
      void Destroy   (){};
      void Delete    (){};
      PlgAdapter<TI, TO> *ref; //ugly stuff

      using ISReceiver<TI>::Create;

   //Slave behaviour (Sender is allowed to Post any time)
      int  Post   (Io *w, TI *m, TimeSpec *ts, IBuffCmd<TI> *c = NULL)
      {
          UNUSED(w);UNUSED(c);
          TO dOut;
          ref->Convert(m, &dOut);
          return ref->out.Send(&dOut, ts);
      };

      void SetLog (IBuffLog<TI> *){}; //TBD?

  private:
      void Create(uint32_t, IBuffCmd<TI> *, ISync *) {}
      int  Receive   (TI *, IBuffCmd<TI> * = NULL) {return 0;};
      int  TryReceive(TI *, IBuffCmd<TI> * = NULL) {return 0;};
};

//#########################################################
template <typename TI, typename TO> PlgAdapter<TI, TO>::PlgAdapter()
{
    in.ref = this;
    Add(&in, ".in");
    Add(&out,".out");
}

#endif
