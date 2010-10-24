

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sat Oct 23 23:59:53 2010
 */
/* Compiler settings for .\SendMail.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SendMail_h_h__
#define __SendMail_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISendMail_FWD_DEFINED__
#define __ISendMail_FWD_DEFINED__
typedef interface ISendMail ISendMail;
#endif 	/* __ISendMail_FWD_DEFINED__ */


#ifndef __SendMail_FWD_DEFINED__
#define __SendMail_FWD_DEFINED__

#ifdef __cplusplus
typedef class SendMail SendMail;
#else
typedef struct SendMail SendMail;
#endif /* __cplusplus */

#endif 	/* __SendMail_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SendMail_LIBRARY_DEFINED__
#define __SendMail_LIBRARY_DEFINED__

/* library SendMail */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SendMail;

#ifndef __ISendMail_DISPINTERFACE_DEFINED__
#define __ISendMail_DISPINTERFACE_DEFINED__

/* dispinterface ISendMail */
/* [uuid] */ 


EXTERN_C const IID DIID_ISendMail;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C5BE5524-3082-42CF-96C2-593FFCD61681")
    ISendMail : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ISendMailVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISendMail * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISendMail * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISendMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISendMail * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISendMail * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISendMail * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISendMail * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISendMailVtbl;

    interface ISendMail
    {
        CONST_VTBL struct ISendMailVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISendMail_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISendMail_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISendMail_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISendMail_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISendMail_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISendMail_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISendMail_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ISendMail_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SendMail;

#ifdef __cplusplus

class DECLSPEC_UUID("676DA748-4C70-45F6-A2C5-50F651227F93")
SendMail;
#endif
#endif /* __SendMail_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


