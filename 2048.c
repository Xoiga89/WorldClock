/*
   Application template for Amazfit Bip BipOS
   (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>

   		World Clock for Amazfit Bip
		(C) Tony Nguyen  2021 <https://direct.me/tonyisme>

*/
// Này là bản cho đồng hồ ở Viet Nam
#include "libbip.h"
#include "2048.h"
#include "BipStatusbarLib.h"

struct regmenu_ screen_data = {
  55,
  1,
  0,
  dispatch_screen,
  key_press_screen,
  screen_job,
  0,
  show_screen,
  0,
  0
};

int main(int param0, char** argv){
  show_screen((void*) param0);
}

void show_screen (void *param0) {
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	pointer to a pointer to screen data
  struct app_data_ *	app_data;					//	pointer to screen data
  if ( (param0 == *app_data_p) && get_var_menu_overlay()){
    app_data = *app_data_p;
    *app_data_p = NULL;
    reg_menu(&screen_data, 0);
    *app_data_p = app_data;
  } else {
    reg_menu(&screen_data, 0);
    *app_data_p = (struct app_data_ *)pvPortMalloc(sizeof(struct app_data_));
    app_data = *app_data_p;
    _memclr(app_data, sizeof(struct app_data_));
    app_data->proc = param0;
    if ( param0 && app_data->proc->ret_f )
      app_data->ret_f = app_data->proc->elf_finish;
    else
      app_data->ret_f = show_watchface;
    
   
    app_data->screen = 1;
 app_data->col=0;
 app_data->current_screen = 0;
 app_data->state_hash = -1;
	
}
draw_screen();

// đặt bộ hẹn giờ để gọi screen_job tính bằng mili giây, tùy thuộc vào việc có cần trình bảo vệ màn hình hay không
set_update_period(1,  1000);


// prevent exiting the app after a period
  set_display_state_value(8, 1);
  set_display_state_value(2, 1);

}

//Da kiem tra oke
void key_press_screen(){
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *	app_data = *app_data_p;

  show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

void screen_job(){
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
  


 if (app_data->screen == 1) {
  draw_board();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
 }   else if ( app_data->screen == 9 ) {
thegioi2();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
  else {

repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }	  
}

int dispatch_screen (void *param){
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *	app_data = *app_data_p;
  struct gesture_ *gest = param;
  int result = 0;
 
  switch (gest->gesture){
    case GESTURE_CLICK: {
                          vibrate(1,50,0);
                          switch (app_data->screen){
                           
									case 8: {if ( app_data->screen == 8 ){ 
				 app_data->screen = 9;
				set_bg_color(COLOR_BLACK);
                fill_screen_bg();
                thegioi2();
                                        repaint_screen_lines(0, 176);
                                      break;
			                          }}
									  case 9: {if ( app_data->screen == 9 ){ 
				 app_data->screen = 1;
				set_bg_color(COLOR_BLACK);
                fill_screen_bg();
                draw_board();
                                        repaint_screen_lines(0, 176);
                                      break;
			                          }}           
                                      break;
                    
	
                          }

                          break;
                        };
    case GESTURE_SWIPE_DOWN: {
                               
                               app_data->screen = 1;
							   draw_board();
							 

                               break;
                             };
    case GESTURE_SWIPE_UP: {
                             
                             break;
                           };
    case GESTURE_SWIPE_LEFT: {
                             
                               show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
                               break;
                             };
    case GESTURE_SWIPE_RIGHT: {
                              
                                                               
								break;
                              };
    default:{
              vibrate(1,70,0);
              break;
            };

  }
  return result;
};


void draw_board(){  // Lấy giờ Úc (Giờ VN +3) , Đức (Giờ VN -6) , Los Angeles (Giờ VN -15) , London (Giờ VN -7). Ký tự nơi ở sẽ thấy kí tự theo Mã chung ISO và cách 15 px, Giờ + thì > 24, Giờ - thì < 0)
  set_bg_color(COLOR_WHITE);
  fill_screen_bg();
  set_fg_color(COLOR_BLACK);
text_out_center("WORLD CLOCK", 88, 6);
text_out_center("TONY", 88, 30);

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT1, 60, 58);
  char clock_time_VN[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time_VN, "%02d:%02d", dt.hour, dt.min);
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_VN, 80 , 58, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     


  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT2, 23, 104);
  char clock_time_GE[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT2 < 0) {
  _sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_GE, 38 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT3, 110, 104);
    char clock_time_AU[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT3 > 24) {
  _sprintf(clock_time_AU, "%02d:%02d", dt.hour + CT3 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_AU, "%02d:%02d", dt.hour + CT3, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_AU, 125 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);    
  
    set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT4, 23, 147);
      char clock_time_US[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT4 < 0) {
  _sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_US, 38 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT5, 110, 147); //Nước anh
      char clock_time_GB[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT5 < 0) {
  _sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_GB, 125 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 


  repaint_screen_lines(0, 176);
 
}

void thegioi2(){  // Lấy giờ Nga (Giờ VN -4) , Trung  (Giờ VN +1) , Nhật (Giờ VN +2) , Argentina (Giờ VN -10). Ký tự nơi ở sẽ thấy kí tự theo Mã chung ISO và cách 15 px, Giờ + thì > 24, Giờ - thì < 0)
  set_bg_color(COLOR_WHITE);
  fill_screen_bg();
  set_fg_color(COLOR_BLACK);
text_out_center("WORLD CLOCK", 88, 6);
text_out_center("TONY", 88, 30);

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT1, 60, 58);
  char clock_time_VN[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time_VN, "%02d:%02d", dt.hour, dt.min);
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_VN, 80 , 58, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     


  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT6, 23, 104);
  char clock_time_RU[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT6 < 0) {
  _sprintf(clock_time_RU, "%02d:%02d", dt.hour + CT6 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_RU, "%02d:%02d", dt.hour + CT6, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_RU, 38 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT7, 110, 104);
    char clock_time_JP[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT7 > 24) {
  _sprintf(clock_time_JP, "%02d:%02d", dt.hour + CT7 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_JP, "%02d:%02d", dt.hour + CT7, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_JP, 125 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);    
  
    set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT8, 23, 147);
      char clock_time_CN[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT8 > 24) {
  _sprintf(clock_time_CN, "%02d:%02d", dt.hour + CT8 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_CN, "%02d:%02d", dt.hour + CT8, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_CN, 38 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT9, 110, 147); //ARGENTINA
      char clock_time_AR[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT9 < 0) {
  _sprintf(clock_time_AR, "%02d:%02d", dt.hour + CT9 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_AR, "%02d:%02d", dt.hour + CT9, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_AR, 125 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 


  repaint_screen_lines(0, 176);
 
}

void draw_screen(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана
 set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
  //set_graph_callback_to_ram_1();
  draw_board();
  
  //set_graph_callback_to_ram_1();
}


