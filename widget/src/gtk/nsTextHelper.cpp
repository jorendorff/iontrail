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

#include <gtk/gtk.h>

#include "nsTextHelper.h"
#include "nsTextWidget.h"
#include "nsToolkit.h"
#include "nsColor.h"
#include "nsGUIEvent.h"
#include "nsString.h"
#include "nsStringUtil.h"

#define DBG 0

//-------------------------------------------------------------------------
//
// nsTextHelper constructor
//
//-------------------------------------------------------------------------

nsTextHelper::nsTextHelper() : nsWidget(), nsITextAreaWidget(), nsITextWidget()
{
  mIsReadOnly = PR_FALSE;
  mIsPassword = PR_FALSE;
}

//-------------------------------------------------------------------------
//
// nsTextHelper destructor
//
//-------------------------------------------------------------------------
nsTextHelper::~nsTextHelper()
{
}

//-------------------------------------------------------------------------
NS_METHOD nsTextHelper::SetMaxTextLength(PRUint32 aChars)
{
// This is a normal entry only thing, not a text box
  gtk_entry_set_max_length(GTK_ENTRY(mWidget), (int)aChars);
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::GetText(nsString& aTextBuffer, PRUint32 aBufferSize, PRUint32& aActualSize)
{
  char *str;
  if (!mIsPassword) {
    if (GTK_IS_ENTRY(mWidget))
    {
      str = gtk_entry_get_text(GTK_ENTRY(mWidget));
    }
    else if (GTK_IS_TEXT(mWidget))
    {
      str = gtk_editable_get_chars (GTK_EDITABLE (mWidget), 0,
                   gtk_text_get_length (GTK_TEXT (mWidget)));
    }
    aTextBuffer.SetLength(0);
    aTextBuffer.Append(str);
    PRUint32 len = (PRUint32)strlen(str);
    aActualSize = len;
  } else {
/*
    PasswordData * data;
    XtVaGetValues(mWidget, XmNuserData, &data, NULL);
    aTextBuffer = data->mPassword;
    aActualSize = aTextBuffer.Length();
*/
  }
  return(NS_OK);
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::SetText(const nsString& aText, PRUint32& aActualSize)
{
  if (!mIsPassword) {
    NS_ALLOC_STR_BUF(buf, aText, 512);

    if (GTK_IS_ENTRY(mWidget)) {
      gtk_entry_set_text(GTK_ENTRY(mWidget), buf);
    } else if (GTK_IS_TEXT(mWidget)) {
      gtk_text_insert(GTK_TEXT(mWidget), nsnull, nsnull, nsnull, buf, aActualSize);
    }

    NS_FREE_STR_BUF(buf);
  } else {
/*
    PasswordData * data;
    XtVaGetValues(mWidget, XmNuserData, &data, NULL);
    data->mPassword = aText;
    data->mIgnore   = PR_TRUE;
    char * buf = new char[aText.Length()+1];
    memset(buf, '*', aText.Length());
    buf[aText.Length()] = 0;
    gtk_entry_set_text(GTK_ENTRY(mWidget), buf);
    data->mIgnore = PR_FALSE;
*/
  }
  aActualSize = aText.Length();

  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::InsertText(const nsString &aText, PRUint32 aStartPos, PRUint32 aEndPos, PRUint32& aActualSize)
{
#if 0
  if (!mIsPassword) {
    NS_ALLOC_STR_BUF(buf, aText, 512);
    XmTextInsert(mWidget, aStartPos, buf);
    NS_FREE_STR_BUF(buf);
  } else {
    PasswordData * data;
    XtVaGetValues(mWidget, XmNuserData, &data, NULL);
    data->mIgnore   = PR_TRUE;
    nsString newText(aText);
    data->mPassword.Insert(newText, aStartPos, aText.Length());
    char * buf = new char[data->mPassword.Length()+1];
    memset(buf, '*', data->mPassword.Length());
    buf[data->mPassword.Length()] = 0;
    XmTextInsert(mWidget, aStartPos, buf);
    data->mIgnore = PR_FALSE;
  }
  aActualSize = aText.Length();
#endif
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::RemoveText()
{
  if (GTK_IS_ENTRY(mWidget)) {
    gtk_entry_set_text(GTK_ENTRY(mWidget), "");
  } else if (GTK_IS_TEXT(mWidget)) {
    gtk_editable_delete_text(GTK_EDITABLE(mWidget), 0,
           gtk_text_get_length(GTK_TEXT (mWidget)));
  }
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::SetPassword(PRBool aIsPassword)
{
  mIsPassword = aIsPassword;
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::SetReadOnly(PRBool aReadOnlyFlag, PRBool& aOldReadOnlyFlag)
{
  NS_ASSERTION(nsnull != mWidget,
               "SetReadOnly - Widget is NULL, Create may not have been called!");
  aOldReadOnlyFlag = mIsReadOnly;
  mIsReadOnly = aReadOnlyFlag;
  gtk_editable_set_editable(GTK_EDITABLE(mWidget), aReadOnlyFlag?PR_FALSE:PR_TRUE);
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsTextHelper::SelectAll()
{
  nsString text;
  PRUint32 actualSize = 0;
  PRUint32 numChars = GetText(text, 0, actualSize);
  SetSelection(0, numChars);
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsTextHelper::SetSelection(PRUint32 aStartSel, PRUint32 aEndSel)
{
  gtk_editable_select_region(GTK_EDITABLE(mWidget), aStartSel, aEndSel);
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsTextHelper::GetSelection(PRUint32 *aStartSel, PRUint32 *aEndSel)
{
#if 0
  XmTextPosition left;
  XmTextPosition right;

  if (XmTextGetSelectionPosition(mWidget, &left, &right)) {
    *aStartSel = (PRUint32)left;
    *aEndSel   = (PRUint32)right;
  } else {
    printf("nsTextHelper::GetSelection Error getting positions\n");
    return NS_ERROR_FAILURE;
  }
#endif
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::SetCaretPosition(PRUint32 aPosition)
{
  gtk_editable_set_position(GTK_EDITABLE(mWidget), aPosition);
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD  nsTextHelper::GetCaretPosition(PRUint32& aPosition)
{
  aPosition = (PRUint32)GTK_EDITABLE(mWidget)->current_pos;
  return NS_OK;
}
