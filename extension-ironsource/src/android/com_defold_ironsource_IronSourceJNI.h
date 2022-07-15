#include <jni.h>
/* Header for class com_defold_ironsource_IronSourceJNI */

#ifndef COM_DEFOLD_IRONSOURCE_IronSourceJNI_H
#define COM_DEFOLD_IRONSOURCE_IronSourceJNI_H
#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     com_defold_ironsource_IronSourceJNI
	* Method:    addToQueue_first_arg
	* Signature: (ILjava/lang/String;I)V
	*/
	JNIEXPORT void JNICALL Java_com_defold_ironsource_IronSourceJNI_addToQueue
		(JNIEnv *, jclass, jint, jstring);

#ifdef __cplusplus
}
#endif
#endif
