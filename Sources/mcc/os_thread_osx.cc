#include "mcc/os_thread.h"
#ifdef OS_IS_OSX
#include <utility>
#include <glog/logging.h>

namespace mcc {
  class ThreadStartData{
  private:
   std::string name_;
   ThreadHandler handler_;
   void* parameter_;
  public:
   ThreadStartData(std::string name, const ThreadHandler& function, void* parameter):
     name_(std::move(name)),
     handler_(function),
     parameter_(parameter){
   }
   ~ThreadStartData() = default;

   std::string GetName() const{
     return name_;
   }

   ThreadHandler& GetFunction(){
     return handler_;
   }

   ThreadHandler GetFunction() const{
     return handler_;
   }

   void* GetParameter() const{
     return parameter_;
   }
 };

 bool SetThreadName(const ThreadId& thread, const char* name){
   char truncated_name[kThreadNameMaxLength];
   snprintf(truncated_name, kThreadNameMaxLength-1, "%s", name);
   int result;
   if((result = pthread_setname_np(truncated_name)) != 0){
     LOG(WARNING) << "couldn't set thread name: " << strerror(result);
     return false;
   }
   return true;
 }

 static void*
 HandleThread(void* pdata){
   auto data = (ThreadStartData*)pdata;
   auto& func = data->GetFunction();
   void* parameter = data->GetParameter();

   if(!SetThreadName(pthread_self(), data->GetName()) != 0)
     goto exit;

   func(parameter);
 exit:
   delete data;
   pthread_exit(nullptr);
 }

 ThreadId GetCurrentThreadId(){
   return pthread_self();
 }

 bool Start(ThreadId* thread, const std::string& name, const ThreadHandler& func, void* parameter){
   int result;
   pthread_attr_t attrs;
   if((result = pthread_attr_init(&attrs)) != 0){
     LOG(ERROR) << "couldn't initialize the thread attributes: " << strerror(result);
     return false;
   }

   DVLOG(1) << "starting " << name << " thread w/ parameter: " << std::hex << parameter;
   auto data = new ThreadStartData(name, func, parameter);
   if((result = pthread_create(thread, &attrs, &HandleThread, data)) != 0){
     LOG(ERROR) << "couldn't start the thread: " << strerror(result);
     return false;
   }

   if((result = pthread_attr_destroy(&attrs)) != 0){
     LOG(ERROR) << "couldn't destroy the thread attributes: " << strerror(result);
     return false;
   }
   return true;
 }

 bool Join(const ThreadId& thread){
   std::string thread_name = GetThreadName(thread);

   char return_data[kThreadMaxResultLength];

   int result;
   if((result = pthread_join(thread, (void**)&return_data)) != 0){
     LOG(ERROR) << "couldn't join thread: " << strerror(result);
     return false;
   }

   VLOG(3) << thread_name << " thread finished w/ result: " << std::string(return_data, kThreadMaxResultLength);
   return true;
 }

 int ThreadEquals(const ThreadId& lhs, const ThreadId& rhs){
   return pthread_equal(lhs, rhs);
 }

 std::string GetThreadName(const ThreadId& thread){
   char name[kThreadNameMaxLength];

   int err;
   if((err = pthread_getname_np(thread, name, kThreadNameMaxLength)) != 0){
     LOG(ERROR) << "cannot get name for " << thread << " thread: " << strerror(err);
     return "unknown";
   }
   return { name };
 }

 bool SetThreadName(const ThreadId& thread, const std::string& name){
   char truncated_name[kThreadNameMaxLength];
   snprintf(truncated_name, kThreadNameMaxLength-1, "%s", name.data());
   int result;
   if((result = pthread_setname_np(truncated_name)) != 0) {
     LOG(WARNING) << "couldn't set thread name: " << strerror(result);
     return false;
   }
   return true;
 }

 bool InitializeThreadLocal(ThreadLocalKey& key){
   int err;
   if((err = pthread_key_create(&key, nullptr)) != 0){//TODO: fix make second parameter visible to caller
     LOG(ERROR) << "failed to create ThreadLocal key: " << strerror(err);
     return false;
   }
   VLOG(3) << "created ThreadLocal key.";
   return true;
 }

 bool SetCurrentThreadLocal(const ThreadLocalKey& key, const void* value){
   int err;
   if((err = pthread_setspecific(key, value)) != 0){
     LOG(ERROR) << "couldn't set " << GetCurrentThreadName() << " ThreadLocal: " << strerror(err);
     return false;
   }
   return true;
 }

 void* GetCurrentThreadLocal(const ThreadLocalKey& key){
   void* ptr;
   if((ptr = pthread_getspecific(key)) != nullptr)
     return ptr;
   return nullptr;
 }
}

#endif//OS_IS_OSX