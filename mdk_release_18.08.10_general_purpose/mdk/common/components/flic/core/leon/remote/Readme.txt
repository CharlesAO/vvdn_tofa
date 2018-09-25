
 ####################
    Call examples:
 ####################

  //"Push" call
   RmiCallCtx ctx;
   ctx->whoToWake = &RmtI.remoteEv;
   ctx->param[0]  = &nonCachedMsg;
   ctx->methodId  = REMOTE_PUSH; //REMOTE_TRYPUSH
   RmiCall(&ctx);

  //Send Resp:
   RmiCallCtx ctx;
   ctx->whoToWake = &RmtO.semResp;
   ctx->methodId  = REMOTE_RESP;
   ctx->param[0]  = rc;
   RmiCall(&outCall);

  //INC_REF:
   ctx->whoToWake = &pool->waitingSem;
   ctx->methodId  = INC_REF;
   ctx->param[0]  = ref;
   ctx->param[1]  = ptr;
   RmiCall(&ctx);
