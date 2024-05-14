#include <gtest/gtest.h>

#include "mcc/mouse/cursor.h"
#include "mcc/mock_subscription.h"

namespace mcc::mouse {
  using namespace ::testing;

  class CursorTest : public Test {
  protected:
    CursorTest() = default;
  public:
    ~CursorTest() override = default;
  };

  TEST_F(CursorTest, Test_ListAvailableCursors) {
    rx::MockSubscription<std::string> sub;
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/bd_double_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/bottom_left_corner.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/bottom_right_corner.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/bottom_side.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/bottom_tee.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/center_ptr.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/circle.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/context-menu.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/copy.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/cross.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/crossed_circle.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/crosshair.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/dnd_no_drop.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/dnd-ask.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/dnd-copy.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/dnd-link.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/dotbox.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/fd_double_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/grabbing.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/hand1.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/hand2.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-01.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-02.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-03.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-04.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-05.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-06.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-07.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-08.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-09.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-10.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-11.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-12.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-13.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-14.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-15.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-16.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-17.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-18.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-19.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-20.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-21.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-22.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-23.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-24.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-25.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-26.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-27.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-28.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-29.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-30.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-31.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-32.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-33.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-34.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-35.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-36.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-37.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-38.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-39.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-40.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-41.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-42.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-43.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-44.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-45.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-46.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-47.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-48.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-49.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-50.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-51.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-52.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-53.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr_watch-54.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_ptr.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_side.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/left_tee.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/link.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/ll_angle.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/lr_angle.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/move.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/pencil.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/person.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/pin.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/plus.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/pointer-move.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/question_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/right_ptr.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/right_side.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/right_tee.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/sb_down_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/sb_h_double_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/sb_left_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/sb_right_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/sb_up_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/sb_v_double_arrow.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/tcross.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/top_left_corner.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/top_right_corner.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/top_side.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/top_tee.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/ul_angle.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/ur_angle.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/vertical-text.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-01.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-02.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-03.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-04.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-05.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-06.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-07.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-08.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-09.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-10.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-11.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-12.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-13.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-14.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-15.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-16.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-17.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-18.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-19.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-20.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-21.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-22.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-23.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-24.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-25.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-26.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-27.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-28.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-29.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-30.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-31.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-32.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-33.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-34.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-35.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-36.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-37.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-38.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-39.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-40.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-41.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-42.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-43.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-44.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-45.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-46.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-47.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-48.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-49.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-50.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-51.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-52.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-53.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wait-54.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/wayland-cursor.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/X_cursor.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/xterm.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/zoom-in.png")));
    EXPECT_CALL(sub, OnNext(StrEq("bibata-modern-ice/zoom-out.png")));

    EXPECT_CALL(sub, OnCompleted());

    const auto cursors = mouse::ListAvailableCursors()
      .filter([](const std::string& name) {
        return StartsWith(name, "bibata-modern-ice/");
      });
    sub.Subscribe(cursors);
  }
}