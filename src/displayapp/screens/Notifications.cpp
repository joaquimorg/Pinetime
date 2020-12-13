#include "Notifications.h"
#include <displayapp/DisplayApp.h>

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(not_email);
LV_IMG_DECLARE(not_instantmessage);
LV_IMG_DECLARE(not_missedcall);
LV_IMG_DECLARE(not_schedule);
LV_IMG_DECLARE(not_sms);
LV_IMG_DECLARE(not_unknown);

LV_IMG_DECLARE(icon_phone);


Notifications::Notifications(DisplayApp *app, Pinetime::Controllers::NotificationManager &notificationManager, Modes mode) :
        Screen(app), notificationManager{notificationManager}, mode{mode} {

  

  // Set the background to Black
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));

  notificationManager.ClearNewNotificationFlag();
  
  auto notification = notificationManager.GetLastNotification();

  if(notification.valid) {
    currentId = notification.id;
    currentItem.reset(new NotificationItem(CategoryToString(notification.category), notification, notification.index, notificationManager.NbNotifications(), mode));
    validDisplay = true;
  } else {
    //currentItem.reset(new NotificationItem("Notification", NULL, 0, notificationManager.NbNotifications(), Modes::Preview));

    lv_obj_t * not_img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(not_img, &icon_phone);
    lv_obj_align(not_img, NULL, LV_ALIGN_CENTER, 0, -60);

    lv_obj_t* label = lv_label_create(lv_scr_act(), nullptr);   
    lv_label_set_recolor(label, true); 
    lv_label_set_text(label, "#0000FF Notification#\n\nNo notification\nto display.");
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
  }

  /*if(mode == Modes::Preview) {
    static lv_style_t style_line;
    //lv_style_init(&style_line);
    //style_line.line.color = LV_COLOR_WHITE;
    //style_line.line.width = 3;
    //style_line.line.rounded = 0;


    //timeoutLine = lv_line_create(lv_scr_act(), nullptr);
    //lv_line_set_style(timeoutLine, LV_LINE_PART_MAIN, &style_line);
    //lv_line_set_points(timeoutLine, timeoutLinePoints, 2);
    //timeoutTickCountStart = xTaskGetTickCount();
    //timeoutTickCountEnd = timeoutTickCountStart + (5*1024);
  }*/
}

Notifications::~Notifications() {
  lv_obj_clean(lv_scr_act());
}


// Unknown, Email, IncomingCall, MissedCall, Sms, Schedule, InstantMessage
char const *Notifications::CategoriesString[] = {
        "Unknown",
        "Email",
        "Incoming Call",
        "Missed Call",
        "Sms",        
        "Schedule",
        "Instant Message"
};

void const *Notifications::CategoriesIcon[] = {
        &not_unknown,
        &not_email,
        &not_missedcall,
        &not_missedcall,
        &not_sms,        
        &not_schedule,
        &not_instantmessage
};

const char* Notifications::CategoryToString( Controllers::NotificationManager::Categories category ) {
  return Notifications::CategoriesString[(uint8_t)category];
}

bool Notifications::Refresh() {
  /*if (mode == Modes::Preview) {
    auto tick = xTaskGetTickCount();
    int32_t pos = 240 - ((tick - timeoutTickCountStart) / ((timeoutTickCountEnd - timeoutTickCountStart) / 240));
    if (pos < 0)
      running = false;

    timeoutLinePoints[1].x = pos;
    lv_line_set_points(timeoutLine, timeoutLinePoints, 2);

    if (!running) {
      // Start clock app when exiting this one
      app->StartApp(Apps::Clock);
    }
  }*/

  return running;
}

bool Notifications::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  switch (event) {
    case Pinetime::Applications::TouchEvents::SwipeUp: {
      Controllers::NotificationManager::Notification previousNotification;
      if(validDisplay)
        previousNotification = notificationManager.GetPrevious(currentId);
      else
        previousNotification = notificationManager.GetLastNotification();

      if (!previousNotification.valid) {        
        if(mode == Modes::Clock || mode == Modes::Preview) {
          running = false;
          app->StartApp(Apps::Clock);
        } else {
          running = false;
        }
        return true;
      }

      validDisplay = true;
      currentId = previousNotification.id;
      currentItem.reset(nullptr);
      app->SetFullRefresh(DisplayApp::FullRefreshDirections::Up);
      currentItem.reset(new NotificationItem(CategoryToString(previousNotification.category), previousNotification,  previousNotification.index, notificationManager.NbNotifications(), mode));
    }
      return true;
    case Pinetime::Applications::TouchEvents::SwipeDown: {
      Controllers::NotificationManager::Notification nextNotification;
      if(validDisplay)
        nextNotification = notificationManager.GetNext(currentId);
      else
        nextNotification = notificationManager.GetLastNotification();

      if (!nextNotification.valid) {
        /*if(mode == Modes::Normal) {
          running = false;
          //app->StartApp(Apps::Launcher);
        }*/
        return true;
      }

      validDisplay = true;
      currentId = nextNotification.id;
      currentItem.reset(nullptr);
      app->SetFullRefresh(DisplayApp::FullRefreshDirections::Down);
      currentItem.reset(new NotificationItem(CategoryToString(nextNotification.category), nextNotification,  nextNotification.index, notificationManager.NbNotifications(), mode));
    }
      return true;
      
    case Pinetime::Applications::TouchEvents::SwipeRight : 
      //running = false;
      // delete ???
      return true;

    default:
      return false;
  }
}


bool Notifications::OnButtonPushed() {
  running = false;
  return true;
}


Notifications::NotificationItem::NotificationItem(const char *title, Controllers::NotificationManager::Notification &msg, uint8_t notifNr, uint8_t notifNb, Modes mode)
        : msg{msg}, notifNr{notifNr}, notifNb{notifNb}, mode{mode} {

  // Set the background to Black
  //lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));

  static lv_style_t style_container;
  lv_style_init(&style_container);
  lv_style_set_bg_color(&style_container, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  //lv_style_set_radius(&style_container, LV_STATE_DEFAULT, 0);
  lv_style_set_pad_all(&style_container, LV_STATE_DEFAULT, 20);
  lv_style_set_border_width(&style_container, LV_STATE_DEFAULT, 0);

  lv_obj_t* container1 = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_add_style(container1, LV_CONT_PART_MAIN, &style_container);

  lv_obj_set_auto_realign(container1, true);                  
  lv_obj_align_origo(container1, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_cont_set_fit(container1, LV_FIT_MAX);
  lv_cont_set_layout(container1, LV_LAYOUT_COLUMN_MID);

  if ( msg.valid ) {

    lv_obj_t * not_img = lv_img_create(container1, NULL);
    lv_img_set_src(not_img, Notifications::CategoriesIcon[(uint8_t)msg.category]);
    lv_obj_align(not_img, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 0);

    lv_obj_t* alert_type = lv_label_create(container1, nullptr);
    //lv_label_set_body_draw(l1, true);
    //lv_obj_set_width(alert_type, LV_HOR_RES - 10);
    lv_label_set_recolor(alert_type, true);
    lv_label_set_text_fmt(alert_type, "#00FF00 %s#", title);

    lv_obj_t* alert_subject = lv_label_create(container1, nullptr);
    lv_label_set_long_mode(alert_subject, LV_LABEL_LONG_BREAK);
    //lv_label_set_body_draw(l1, true);
    lv_obj_set_width(alert_subject, LV_HOR_RES - 20);
    lv_label_set_recolor(alert_subject, true);
    lv_label_set_text_fmt(alert_subject, "#AAAAAA %s#", msg.subject.data());

    lv_obj_t* alert_body = lv_label_create(container1, nullptr);
    lv_label_set_long_mode(alert_body, LV_LABEL_LONG_BREAK);
    //lv_label_set_body_draw(l1, true);
    lv_obj_set_width(alert_body, LV_HOR_RES - 20);
    lv_label_set_text(alert_body, msg.message.data());

    lv_obj_t* alert_time = lv_label_create(container1, nullptr);
    //lv_label_set_body_draw(l1, true);
    //lv_obj_set_width(alert_type, LV_HOR_RES - 10);
    lv_label_set_recolor(alert_time, true);
    lv_label_set_text_fmt(alert_time, "#0000FF %s:%s#", msg.hour.data(), msg.minute.data());

  } else {
    lv_obj_t* alert_type = lv_label_create(container1, nullptr);
    //lv_label_set_body_draw(l1, true);
    lv_obj_set_width(alert_type, LV_HOR_RES - 20);
    lv_label_set_text_fmt(alert_type, "$s\n\n$s", title, "Invalid alert...");
  }

}


Notifications::NotificationItem::~NotificationItem() {
  lv_obj_clean(lv_scr_act());
}
