/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */


#include "nsIBlockingNotification.h"
#include "nsIURL.h"

#include "nsIEventQueueService.h"
#include "nsIServiceManager.h"
#include "nsXPComFactory.h"
#include "nsXPComCIID.h"

// Class IDs...
static NS_DEFINE_IID(kEventQueueServiceCID,  NS_EVENTQUEUESERVICE_CID);

// Interface IDs...
static NS_DEFINE_IID(kIEventQueueServiceIID, NS_IEVENTQUEUESERVICE_IID);
static NS_DEFINE_IID(kIBlockingNotificationObserverIID, 
                                             NS_IBLOCKINGNOTIFICATION_OBSERVER_IID);

// Forward declarations...
class nsDefaultProtocolHelper;



/*-------------------- Notification Event Class ----------------------------*/

struct NotificationEvent : public PLEvent 
{
  NotificationEvent(nsDefaultProtocolHelper *aSelf,
                    nsIBlockingNotification *aCaller, 
                    nsIURL *aUrl, 
                    PRInt32 aCode, 
                    nsISupports *aExtraInfo);
  ~NotificationEvent();
  PRStatus Fire(PLEventQueue* aEventQ);

  static void PR_CALLBACK HandlePLEvent(PLEvent* aEvent);
  static void PR_CALLBACK DestroyPLEvent(PLEvent* aEvent);

  nsDefaultProtocolHelper *mSelf;
  nsIBlockingNotification *mCaller;
  nsIURL *mUrl;
  PRInt32 mCode;
  nsISupports *mExtraInfo;
};



/*----------------------------- nsDefaultProtocolHelper ---------------------*/




class nsDefaultProtocolHelper : public nsIBlockingNotificationObserver
{
public:
  nsDefaultProtocolHelper();

  /* nsISupports interface... */
  NS_DECL_ISUPPORTS

  /* nsIBlockingNotificationObserver interface... */
  NS_IMETHOD Notify(nsIBlockingNotification *aCaller,
                    nsIURL *aUrl,
                    PRThread *aThread,
                    PRInt32 aCode,
                    nsISupports *aExtraInfo);

  NS_IMETHOD CancelNotify(nsIURL *aUrl);

  /* class methods... */
  nsresult HandleNotification(nsIBlockingNotification *aCaller,
                              nsIURL *aUrl,
                              PRInt32 aCode,
                              nsISupports *aExtraInfo);

protected:
  virtual ~nsDefaultProtocolHelper();

private:
  nsIEventQueueService *mEventQService;
};


nsDefaultProtocolHelper::nsDefaultProtocolHelper()
{
  NS_INIT_REFCNT();

  /* 
   * Cache the EventQueue service... 
   *
   * Ignore failure since there is nothing that can be done...
   * Instead, all of the code that uses mEventQService *must*
   * check that it was initialized...
   */
  (void) nsServiceManager::GetService(kEventQueueServiceCID,
                                      kIEventQueueServiceIID,
                                      (nsISupports **)&mEventQService);
}

nsDefaultProtocolHelper::~nsDefaultProtocolHelper()
{
  if (mEventQService) {
    nsServiceManager::ReleaseService(kEventQueueServiceCID, mEventQService);
    mEventQService = nsnull;
  }
}

/* 
 * Implementations of nsISupports interface methods...
 */
NS_IMPL_ADDREF(nsDefaultProtocolHelper);
NS_IMPL_RELEASE(nsDefaultProtocolHelper);
NS_IMPL_QUERY_INTERFACE(nsDefaultProtocolHelper, kIBlockingNotificationObserverIID);



/*
 * Implementations of nsIBlockingNotificationObserver interface methods... 
 */
NS_IMETHODIMP
nsDefaultProtocolHelper::Notify(nsIBlockingNotification *aCaller,
                                nsIURL *aUrl,
                                PRThread *aThread,
                                PRInt32 aCode,
                                nsISupports *aExtraInfo)
{
  nsresult rv;
  NotificationEvent *ev;

  /* 
   * Initialize the return code to indicate that the notification was not
   * processed synchronously...
   */
  rv = NS_NOTIFY_BLOCKED;
    
  /*
   * If no thread switch is necessary, then handle the notification
   * immediately...
   */
  if (PR_GetCurrentThread() == aThread) {
    rv = HandleNotification(aCaller, aUrl, aCode, aExtraInfo);
  }
  else {
    /*
     * Post a message to the appropriate thread event queue to
     * handle the notification...
     */
    PLEventQueue *evQ = nsnull;

    /* locate the event queue for the thread... */
    if (mEventQService) {
      mEventQService->GetThreadEventQueue(aThread, &evQ);
    }

    /* Create and dispatch the notification event... */
    if (evQ) {
      ev = new NotificationEvent(this, aCaller, aUrl, aCode, aExtraInfo);
      if (ev) {
        PRStatus status;

        /* dispatch the event into the appropriate event queue... */
        status = ev->Fire(evQ);

        if (PR_SUCCESS != status) {
          /* If the event was not dispatched, then clean up... */
          NotificationEvent::DestroyPLEvent(ev);
          rv = NS_ERROR_FAILURE;
        }
      }
      else {
        /* allocation of the Notification event failed... */ 
        rv = NS_ERROR_OUT_OF_MEMORY;
      }
    } else {
      /* No event queue was found! */
      NS_ASSERTION(0, "No Event Queue is available!");
      rv = NS_ERROR_FAILURE;
    }
  }
  return rv;
}


NS_IMETHODIMP
nsDefaultProtocolHelper::CancelNotify(nsIURL *aUrl)
{
  /* XXX: does not support interrupting a notification yet... */
  return NS_ERROR_FAILURE;
}


nsresult nsDefaultProtocolHelper::HandleNotification(nsIBlockingNotification *aCaller,
                                                     nsIURL *aUrl,
                                                     PRInt32 aCode,
                                                     nsISupports *aExtraInfo)
{


  return NS_NOTIFY_BLOCKED;
}



/*-------------------- Notification Event Class ----------------------------*/



NotificationEvent::NotificationEvent(nsDefaultProtocolHelper *aSelf,
                                     nsIBlockingNotification *aCaller,
                                     nsIURL *aUrl,
                                     PRInt32 aCode,
                                     nsISupports *aExtraInfo)
{
  mSelf      = aSelf;
  mCaller    = aCaller;
  mUrl       = aUrl;
  mCode      = aCode;
  mExtraInfo = aExtraInfo;

  NS_IF_ADDREF(mSelf);
  NS_IF_ADDREF(mCaller);
  NS_IF_ADDREF(mUrl);
  NS_IF_ADDREF(mExtraInfo);
}


NotificationEvent::~NotificationEvent()
{
  NS_IF_RELEASE(mSelf);
  NS_IF_RELEASE(mCaller);
  NS_IF_RELEASE(mUrl);
  NS_IF_RELEASE(mExtraInfo);
}

void PR_CALLBACK NotificationEvent::HandlePLEvent(PLEvent* aEvent)
{
  NotificationEvent *ev = (NotificationEvent*)aEvent;

  (void)ev->mSelf->HandleNotification(ev->mCaller, ev->mUrl, ev->mCode, 
                                      ev->mExtraInfo);
}

void PR_CALLBACK NotificationEvent::DestroyPLEvent(PLEvent* aEvent)
{
  NotificationEvent *ev = (NotificationEvent*)aEvent;

  delete ev;
}

PRStatus NotificationEvent::Fire(PLEventQueue* aEventQ) 
{
  PRStatus status;

  PL_InitEvent(this, nsnull,
               (PLHandleEventProc)  NotificationEvent::HandlePLEvent,
               (PLDestroyEventProc) NotificationEvent::DestroyPLEvent);

  status = PL_PostEvent(aEventQ, this);
  return status;
}



//----------------------------------------------------------------------

// Entry point to create nsAppShellService factory instances...
NS_DEF_FACTORY(DefaultProtocolHelper,nsDefaultProtocolHelper)

nsresult NS_NewDefaultProtocolHelperFactory(nsIFactory** aResult)
{
  nsresult rv = NS_OK;
  nsIFactory* inst;
  
  inst = new nsDefaultProtocolHelperFactory;
  if (nsnull == inst) {
    rv = NS_ERROR_OUT_OF_MEMORY;
  }
  else {
    NS_ADDREF(inst);
  }
  *aResult = inst;
  return rv;
}
