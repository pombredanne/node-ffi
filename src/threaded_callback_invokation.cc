#include "ffi.h"

ThreadedCallbackInvokation::ThreadedCallbackInvokation(CallbackInfo *cbinfo, void *retval, void **parameters) {
  m_cbinfo = cbinfo;
  m_retval = retval;
  m_parameters = parameters;

  pthread_mutex_init(&m_mutex, NULL);
  pthread_cond_init(&m_cond, NULL);
  uv_ref(uv_default_loop()); // hold the event loop open while this is executing
}

ThreadedCallbackInvokation::~ThreadedCallbackInvokation() {
  uv_unref(uv_default_loop());
  pthread_cond_destroy(&m_cond);
  pthread_mutex_destroy(&m_mutex);
}

void ThreadedCallbackInvokation::SignalDoneExecuting() {
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
}

void ThreadedCallbackInvokation::WaitForExecution() {
  pthread_mutex_lock(&m_mutex);
  pthread_cond_wait(&m_cond, &m_mutex);
  pthread_mutex_unlock(&m_mutex);
}
