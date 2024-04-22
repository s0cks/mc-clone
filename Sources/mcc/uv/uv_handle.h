#ifndef MCC_UV_HANDLE_H
#define MCC_UV_HANDLE_H

#include <uv.h>

namespace mcc::uv {
  class HandleBase {
  protected:
    HandleBase() = default;

    template<typename H, typename D>
    static inline void
    SetHandleData(H* handle, const D* data) {
      return uv_handle_set_data((uv_handle_t*) handle, (void*) data);
    }

    template<typename H, typename D>
    static inline void
    SetHandleData(H& handle, const D* data) {
      return SetHandleData<H>(&handle, data);
    }

    template<typename D, typename H>
    static inline D*
    GetHandleData(const H* handle) {
      return (D*) uv_handle_get_data((uv_handle_t*) handle);
    }
  public:
    virtual ~HandleBase() = default;
  };

  template<typename H>
  class HandleBaseTemplate : public HandleBase {
  protected:
    H handle_;

    HandleBaseTemplate() = default;
  public:
    ~HandleBaseTemplate() override = default;
  };
}

#include "mcc/uv/uv_handle_async.h"
#include "mcc/uv/uv_handle_check.h"
#include "mcc/uv/uv_handle_idle.h"
#include "mcc/uv/uv_handle_prepare.h"

#endif //MCC_UV_HANDLE_H