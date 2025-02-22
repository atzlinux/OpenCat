#include "ld3320_ch/asr_ld3320.h"
long voiceTimer;

// format:
//    phrase:token+command
//    should NOT be more than 40 chars
const char voice1[] PROGMEM = "ma pi jing:m0 50 0 -10 0 30";                                                                                                                                                                                                        //马屁精
const char voice2[] PROGMEM = "ni hao:khi";                                                                                                                                                                                                                         //你好
const char voice3[] PROGMEM = "qi li:kup";                                                                                                                                                                                                                          //起立
const char voice4[] PROGMEM = "zuo xia:ksit";                                                                                                                                                                                                                       //坐下
const char voice5[] PROGMEM = "fu wo cheng:kpu";                                                                                                                                                                                                                    //俯卧撑
const char voice6[] PROGMEM = "jia you:kjy";                                                                                                                                                                                                                        //加油
const char voice7[] PROGMEM = "ta bu:kvt";                                                                                                                                                                                                                          //踏步
const char voice8[] PROGMEM = "xiao xing xing:b14,8,14,8,21,8,21,8,23,8,23,8,21,4,19,8,19,8,18,8,18,8,16,8,16,8,14,4,21,8,21,8,19,8,19,8,18,8,18,8,16,4,21,8,21,8,19,8,19,8,18,8,18,8,16,4,14,8,14,8,21,8,21,8,23,8,23,8,21,4,19,8,19,8,18,8,18,8,16,8,16,8,14,4";  //小星星
const char voice9[] PROGMEM = "shui jiao:d ";                                                                                                                                                                                                                       //睡觉
const char voice10[] PROGMEM = "guan cha:kck";                                                                                                                                                                                                                      //观察
const char voice11[] PROGMEM = "he lou:khi";                                                                                                                                                                                                                        //hello
const char voice12[] PROGMEM = "si dan de a pu:kup";                                                                                                                                                                                                                //stand up
const char voice13[] PROGMEM = "yao tou:m0 -45 0 45 0 -45 0 45";                                                                                                                                                                                                    //stand up
const char voice14[] PROGMEM = "chong fu:T ";
const char *const voiceTable[] PROGMEM = { voice1, voice2, voice3, voice4,
                                           voice5, voice6, voice7, voice8,
                                           voice9, voice10, voice11, voice12, voice13, voice14 };

void voiceLD3320Setup() {
  PTL("Init LD3320 voice");          // ld3320
  E_WORK_MODE asr_mode = LOOP_MODE;  // 0：循环识别模式  1：口令模式，以第一个词条为口令  2、按键模式，按下开始识别
  ld3320_reset();
  ld3320_config_mode(asr_mode);  // 循环模式

  for (byte v = 0; v < sizeof(voiceTable) / 2; v++) {
    char *ptr;
    strcpy_P(newCmd, (char *)pgm_read_word(&(voiceTable[v])));
    ptr = strtok(newCmd, ":");
    ld3320_add_words(v, ptr);

    PTL(v);
  }
  //  ld3320_add_words(0, "ma pi jing");  // 马屁精
  //  ld3320_add_words(1, "ni hao");      // 你好
  //  ld3320_add_words(2, "qi li");       // 开灯
  //  ld3320_add_words(3, "zuo xia");     // 坐下
  //  ld3320_add_words(4, "xiu xi");      //休息
  //  ld3320_add_words(5, "jia you");     // 加油
  //  ld3320_add_words(6, "ta bu");       // 踏步
  //  ld3320_add_words(7, "liang zhi lao hu");    // 啦啦啦
  //  ld3320_add_words(8, "shui jiao");   // 睡觉
  //  ld3320_add_words(9, "guan cha");    // 观察
  ld3320_asr_start();
}

void read_voice_ld3320() {
  if (millis() - voiceTimer > 200) {
    unsigned char result;
    result = ld3320_get_result();
    if (result != 0xFF) {
      PT("asr result is:");
      PTL(result);  //返回识别结果，即识别到的词条编号
      for (byte v = 0; v < sizeof(voiceTable) / 2; v++) {
        if (result == v) {
          char *ptr;
          strcpy_P(newCmd, (char *)pgm_read_word(&(voiceTable[v])));
          ptr = strtok(newCmd, ":");
          ptr = strtok(NULL, ":");
          token = ptr[0];
          if (token == T_SKILL)
            strcpy(newCmd, ptr + 1);
          else
            strcpy(newCmd, ptr + 1);
          break;
        }
      }
      newCmdIdx = 6;
      voiceTimer = millis();
    }
  }
}
