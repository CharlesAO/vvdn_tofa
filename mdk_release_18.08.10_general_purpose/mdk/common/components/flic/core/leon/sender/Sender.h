///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Master/Slave Sender
///

#ifndef __SENDER_H__
#define __SENDER_H__

#include <Flic.h>
#include <Receiver.h>

template<typename T> class ISReceiver;
template<typename T> class MReceiver;

//###############################################################
// Master-Sender
//###############################################################
template<typename T> class MSender : public Io
{
   public:
        ISReceiver<T> **recs;
        uint32_t       nRecs; //actual number of recs
        uint32_t       mRecs; //max number of recs
        uint8_t       *errNo; //POSIX compliant error codes (fits 8 bit)

        MSender(uint32_t maxRecs = MAX_RECS_PER_SENDER)
        {
           nRecs = 0;
           recs  = new ISReceiver<T>* [maxRecs];
           errNo = new uint8_t        [maxRecs];
           mRecs = maxRecs;
        }
        virtual ~MSender(){
            delete[] recs;
            delete[] errNo;
        }
        void Delete(){nRecs = 0;}

        void Link   (ISReceiver<T> *r);
        int  Send   (T  *msg);
        int  Send   (T  *msg, TimeSpec *ts, TsType = REL);
        int  TrySend(T  *msg);
        int  HasErr (uint8_t errCode);
};

template <typename T> void MSender<T>::Link(ISReceiver<T> *r)
{
    CheckParent(this);
    CheckParent(r);
   #if !defined(CANCEL_IO_PARENT_CHECK)
    assert(this->par->pl == r->par->pl);
   #endif

    assert(nRecs < mRecs);
    recs[nRecs] = r;
    nRecs++;

    if(FLIC_PLOG && par) //LOGGING
     if((par->pl) && (r->par))
      if(par->pl->log)
       par->pl->log->Print("Pipe [%.8s] MS_LINK [%.8s%.8s]->[%.8s%.8s]\n",
        (char*)&par->pl->name, (char*)&par->name, (char*)&name,
        (char*)&r->par->name, (char*)&r->name);
}

template <typename T> int MSender<T>::Send(T *msg, TimeSpec *ts, TsType tt)
{
    int rc = OK;
    TimeSpec absTs = GetAbsTs(ts, tt);
    for(uint32_t i=0; i<nRecs; i++){
        errNo[i] = recs[i]->Post(this, msg, &absTs);
        rc |= errNo[i];
    }
    return rc;
}

template <typename T> int MSender<T>::Send(T *msg){
  return Send(msg, &TS_INF, ABS);
}

template <typename T> int MSender<T>::TrySend(T *msg){
  return Send(msg, &TS_TRY, ABS);
}

template <typename T> int MSender<T>::HasErr(uint8_t errCode){
  for(uint32_t i=0; i<nRecs; i++)
   if(errNo[i] == errCode) return 1;
  return 0; //else: not found
}

//###############################################################
//Slave-Sender (responds to Master-Receiver requests)
//###############################################################
template <typename T> class SSender : public Io
{
    friend class MReceiver<T>;

    public: void Link(MReceiver<T> *rec)
           {
              CheckParent(this);
              CheckParent(rec);
             #if !defined(CANCEL_IO_PARENT_CHECK)
              assert(this->par->pl == rec->par->pl);
             #endif

              rec->snd = this;

              if(FLIC_PLOG && par) //LOGGING
               if((par->pl) && (rec->par))
                if(par->pl->log)
                 par->pl->log->Print("Pipe [%.8s] SS_LINK [%.8s%.8s]->[%.8s%.8s]\n",
                  (char*)&par->pl->name,  (char*)&par->name,  (char*)&name,
                  (char*)&rec->par->name, (char*)&rec->name);
           }

    protected: //only Receiver can invoke "Pull"
     virtual int Pull   (Io *who, T *msg, TimeSpec *ts, IBuffCmd<T> *c) = 0;
     virtual ~SSender(){};
};
#endif
