/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
*/

#include "ccefeventstransformer.h"
#include "common/Types.h"
//#include "regex"
#include "ccefeventsimpl.cpp"

#include <QDebug>
using namespace NSEditorApi;

CCefEventsTransformer::CCefEventsTransformer(QObject *parent)
    : CAscMenuEventListener()
    , m_propCls()
{
    pObjParent = parent;

    qRegisterMetaType<std::wstring>("std::wstring");
}

void CCefEventsTransformer::OnEvent(NSEditorApi::CAscMenuEvent *pEvent)
{
    if (NULL == pEvent || NULL == pObjParent)
        return;

    if ( m_propCls->onEvent(pObjParent, pEvent) )
        return;

    switch (pEvent->m_nType) {
    case ASC_MENU_EVENT_TYPE_CEF_CREATETAB: {
        CAscCreateTab * pData = (CAscCreateTab *)pEvent->m_pData;

        QMetaObject::invokeMethod(pObjParent, "onCloudDocumentOpen", Qt::QueuedConnection,
                Q_ARG(std::wstring, pData->get_Url()), Q_ARG(int, pData->get_IdEqual()), Q_ARG(bool, pData->get_Active()));

        break;}

    case ASC_MENU_EVENT_TYPE_CEF_TABEDITORTYPE: {
        CAscTabEditorType * pData = (CAscTabEditorType *)pEvent->m_pData;

        QMetaObject::invokeMethod(pObjParent, "onDocumentType", Qt::QueuedConnection, Q_ARG(int, pData->get_Id()), Q_ARG(int, pData->get_Type()));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_ONCLOSE: break;
    case ASC_MENU_EVENT_TYPE_CEF_DOCUMENT_NAME: {
        CAscDocumentName * pData = (CAscDocumentName *)pEvent->m_pData;

        ADDREFINTERFACE(pData)
        QMetaObject::invokeMethod(pObjParent, "onDocumentName", Qt::QueuedConnection, Q_ARG(void *, pData));
        break; }

    case ASC_MENU_EVENT_TYPE_CEF_MODIFY_CHANGED: {
        NSEditorApi::CAscDocumentModifyChanged * pData = (NSEditorApi::CAscDocumentModifyChanged *)pEvent->m_pData;

        QMetaObject::invokeMethod(pObjParent, "onDocumentChanged", Qt::QueuedConnection,
                                    Q_ARG(int, pData->get_Id()), Q_ARG(bool, pData->get_Changed()));

        break;}

    case ASC_MENU_EVENT_TYPE_CEF_ONSAVE: {
        CAscDocumentOnSaveData * pData = (CAscDocumentOnSaveData *)pEvent->m_pData;
        QMetaObject::invokeMethod(pObjParent, "onDocumentSave", Qt::QueuedConnection,
                                  Q_ARG(int, pData->get_Id()), Q_ARG(bool, pData->get_IsCancel()));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_ONLOGOUT: {
        /* will be used synchronous action after logout immediately */
//        QMetaObject::invokeMethod(pObjParent, "onLogout", Qt::QueuedConnection);
        break;}

//    case ASC_MENU_EVENT_TYPE_CEF_JS_MESSAGE: { // deprecated
//        NSEditorApi::CAscJSMessage * pData = (NSEditorApi::CAscJSMessage *)pEvent->m_pData;
//        QString cmd = QString::fromStdWString(pData->get_Name());
//        if (cmd.compare("login") == 0) {
//            QMetaObject::invokeMethod(pObjParent, "onLogin", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdWString(pData->get_Value())));
//        }
//        break; }

    case ASC_MENU_EVENT_TYPE_CEF_ONBEFORECLOSE: break;
    case ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_PROGRESS: break;

//    case ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD_START: deprecated
    case ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD: {
        NSEditorApi::CAscDownloadFileInfo * pData = (NSEditorApi::CAscDownloadFileInfo *)pEvent->m_pData;

        ADDREFINTERFACE(pData)
        QMetaObject::invokeMethod(pObjParent, "onDocumentDownload", Qt::QueuedConnection, Q_ARG(void *, pData));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_END: {
        NSEditorApi::CAscPrintEnd * pData = (NSEditorApi::CAscPrintEnd *)pEvent->m_pData;

        ADDREFINTERFACE(pData)
        QMetaObject::invokeMethod(pObjParent, "onDocumentPrint", Qt::QueuedConnection, Q_ARG(void *, pData));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_ONOPENLINK: {
        NSEditorApi::CAscOnOpenExternalLink * pData = (NSEditorApi::CAscOnOpenExternalLink *)pEvent->m_pData;
        QMetaObject::invokeMethod(pObjParent, "onLink", Qt::QueuedConnection, Q_ARG(QString, QString().fromStdWString(pData->get_Url())));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_ONKEYBOARDDOWN: {
        NSEditorApi::CAscKeyboardDown * pData = (NSEditorApi::CAscKeyboardDown *)pEvent->m_pData;

        ADDREFINTERFACE(pData)
        QMetaObject::invokeMethod(pObjParent, "onKeyDown", Qt::QueuedConnection, Q_ARG(void *, pData));
        break; }

    case ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENENTER:
    case ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENLEAVE:
        QMetaObject::invokeMethod(pObjParent, "onFullScreen", Qt::QueuedConnection,
                        Q_ARG(bool, pEvent->m_nType == ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENENTER));
        break;
    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILE_OPEN: {
        CAscLocalFileOpen * pData = (CAscLocalFileOpen*)pEvent->m_pData;
        QMetaObject::invokeMethod( pObjParent, "onLocalFileOpen", Qt::QueuedConnection,
                                   Q_ARG(QString, QString().fromStdWString(pData->get_Directory())) );
        break;}
    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILES_OPEN: {
        CAscLocalOpenFiles * pData = (CAscLocalOpenFiles *)pEvent->m_pData;
        ADDREFINTERFACE(pData);

        QMetaObject::invokeMethod(pObjParent, "onLocalFilesOpen", Qt::QueuedConnection, Q_ARG(void *, pData));
        break; }

    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILE_CREATE: {
        CAscLocalFileCreate * pData = (CAscLocalFileCreate *)pEvent->m_pData;
        QMetaObject::invokeMethod(pObjParent, "onLocalFileCreate", Qt::QueuedConnection, Q_ARG(int, pData->get_Type()));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILE_RECOVEROPEN:
    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILE_RECENTOPEN: {
        CAscLocalOpenFileRecent_Recover * pData = (CAscLocalOpenFileRecent_Recover *)pEvent->m_pData;

        ADDREFINTERFACE(pData);
        QMetaObject::invokeMethod(pObjParent, "onLocalFileRecent", Qt::QueuedConnection, Q_ARG(void *, pData));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILE_SAVE: {
        CAscLocalSaveFileDialog * pData = (CAscLocalSaveFileDialog *)pEvent->m_pData;

        ADDREFINTERFACE(pData);
        QMetaObject::invokeMethod(pObjParent, "onLocalFileSaveAs", Qt::QueuedConnection, Q_ARG(void *, pData));
        break;}

    case ASC_MENU_EVENT_TYPE_CEF_LOCALFILE_ADDIMAGE: {
        CAscLocalOpenFileDialog * pData = (CAscLocalOpenFileDialog *)pEvent->m_pData;

        ADDREFINTERFACE(pData);
//        emit signal_LocalFile_AddImage(pData);
        QMetaObject::invokeMethod(pObjParent, "onLocalGetImage", Qt::QueuedConnection, Q_ARG(void *, pData));

        break;}
    case ASC_MENU_EVENT_TYPE_CEF_EXECUTE_COMMAND: {
        CAscExecCommand * pData = (CAscExecCommand *)pEvent->m_pData;
        std::wstring cmd = pData->get_Command();

        if (cmd.compare(L"portal:open") == 0) {
            QMetaObject::invokeMethod( pObjParent, "onPortalOpen", Qt::QueuedConnection,
                    Q_ARG(QString, QString::fromStdWString(pData->get_Param())) );
        } else
        if ( !(cmd.find(L"portal:new") == std::wstring::npos) ) {
            QMetaObject::invokeMethod( pObjParent, "onPortalNew", Qt::QueuedConnection,
                    Q_ARG(QString, QString::fromStdWString(pData->get_Param())) );
        } else
        if ( !(cmd.find(L"portal:create") == std::wstring::npos) ) {
            QMetaObject::invokeMethod( pObjParent, "onPortalCreate", Qt::QueuedConnection);
        } else
        if ( !(cmd.find(L"portal:login") == std::wstring::npos) ) {
            QMetaObject::invokeMethod( pObjParent, "onPortalLogin", Qt::QueuedConnection,
                    Q_ARG(QString, QString::fromStdWString(pData->get_Param())) );
        } else
        if (cmd.compare(L"portal:logout") == 0) {
            QMetaObject::invokeMethod( pObjParent, "onPortalLogout", Qt::QueuedConnection,
                    Q_ARG(QString, QString::fromStdWString(pData->get_Param())) );
        } else
        if ( !(cmd.find(L"files:check") == std::wstring::npos) ) {
            QMetaObject::invokeMethod( pObjParent, "onLocalFilesCheck", Qt::QueuedConnection,
                    Q_ARG(QString, QString::fromStdWString(pData->get_Param())) );
        } else
        if ( !(cmd.find(L"files:explore") == std::wstring::npos) ) {
                QMetaObject::invokeMethod( pObjParent, "onLocalFileLocation", Qt::QueuedConnection,
                    Q_ARG(QString, QString::fromStdWString(pData->get_Param())) );
        } else {
//            std::wregex _re_appcmd(L"^app\\:(\\w+)", std::tr1::wregex::icase);
//            auto _iter_cmd = std::wsregex_iterator(cmd.begin(), cmd.end(), _re_appcmd);
//            if (_iter_cmd != std::wsregex_iterator()) {
//                std::wsmatch _cmd_match = *_iter_cmd;

//                cmd = _cmd_match.str(1);
                if (cmd.find(L"app:onready") != std::wstring::npos) {
                    QMetaObject::invokeMethod( pObjParent, "onMainPageReady", Qt::QueuedConnection );
                } else
                if (cmd.find(L"app:buynow") != std::wstring::npos) {
                    QMetaObject::invokeMethod( pObjParent, "onBuyNow", Qt::QueuedConnection );
                }
//            }
        }

        break;
    }
    }

    RELEASEINTERFACE(pEvent);
}
