//#include "stm32f10x.h"                  // Device header
//// 共阳极数码管段码表（0~9，不带小数点，bit0=a, bit1=b...bit6=g, bit7=dp，0亮1灭）
//const uint8_t seg_code_table[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

///************************* 全局变量定义 *************************/
//uint8_t  run_flag = 0;      // 运行标志：0=停止，1=运行
//uint16_t ms_count = 0;      // 1ms计数，累计到10为0.01秒
//uint8_t  cent_sec = 0;      // 百分位 0~9
//uint8_t  dec_sec = 0;       // 十分位 0~9
//uint8_t  sec_unit = 0;      // 秒个位 0~9
//uint8_t  sec_tens = 0;      // 秒十位 0~9
//uint8_t  seg_index = 0;     // 数码管当前扫描位 0~3（对应S1~S4）
//uint8_t  seg_buf[4];        // 数码管显示缓冲区，存储4位的段码
//uint16_t flash_count = 0;   // 小数点闪烁计数，累计到500ms翻转状态
//uint8_t  dp_flag = 0;       // 小数点显示标志：0=灭，1=亮
//uint8_t  send_flag = 0;     // 串口发送标志：1=需要上传当前计时值

///************************* 函数声明 *************************/
//void SEG_Write(uint8_t seg_code);
//void SEG_Select(uint8_t index);
///* USER CODE END 0 */


///* USER CODE BEGIN 4 */
///**
// * @brief  输出段码到数码管段引脚
// * @param  seg_code: 段码，bit0=a ~ bit6=g, bit7=dp，共阳逻辑0亮1灭
// * @retval None
// */
//void SEG_Write(uint8_t seg_code)
//{
//    // 输出a~g段到PC8~PC14
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8,  (seg_code & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9,  (seg_code & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (seg_code & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, (seg_code & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, (seg_code & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, (seg_code & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, (seg_code & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    // 输出小数点dp到PD2
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2,  (seg_code & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//}

///**
// * @brief  选通对应位数码管，关闭其他位
// * @param  index: 位索引 0~3，对应S1(十位)~S4(百分位)
// * @retval None
// */
//void SEG_Select(uint8_t index)
//{
//    // 先关闭所有位，防止残影
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

//    // 选通目标位（PNP三极管基极低电平导通）
//    switch(index)
//    {
//        case 0: HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); break; // 十位S1
//        case 1: HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); break; // 个位S2（带小数点）
//        case 2: HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); break; // 十分位S3
//        case 3: HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); break; // 百分位S4
//        default: break;
//    }
//}

///**
// * @brief  TIM2更新中断回调，1ms进入一次
// * @param  htim: 定时器句柄
// * @retval None
// */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim->Instance == TIM2)
//    {
//        /******************** 1. 数码管动态扫描 ********************/
//        SEG_Select(0xFF); // 先关闭所有位，避免切换残影
//        seg_index = (seg_index + 1) % 4; // 切换扫描位
//        SEG_Write(seg_buf[seg_index]);   // 输出当前位段码
//        SEG_Select(seg_index);           // 选通当前位

//        /******************** 2. 小数点闪烁（走停均生效） ********************/
//        flash_count++;
//        if(flash_count >= 500) // 500ms翻转一次，每秒闪烁1次
//        {
//            flash_count = 0;
//            dp_flag = !dp_flag;
//        }

//        /******************** 3. 计时逻辑（仅运行时生效） ********************/
//        if(run_flag == 1)
//        {
//            ms_count++;
//            if(ms_count >= 10) // 10ms = 0.01秒，精度达标
//            {
//                ms_count = 0;
//                cent_sec++; // 百分位+1
//                if(cent_sec >= 10) { cent_sec = 0; dec_sec++; }
//                if(dec_sec >= 10)  { dec_sec = 0;  sec_unit++; }
//                if(sec_unit >= 10) { sec_unit = 0; sec_tens++; }
//                if(sec_tens >= 10) { sec_tens = 0; } // 99.99秒溢出清零
//            }
//        }

//        /******************** 4. 更新显示缓冲区 ********************/
//        seg_buf[0] = seg_code_table[sec_tens];
//        seg_buf[1] = seg_code_table[sec_unit];
//        seg_buf[2] = seg_code_table[dec_sec];
//        seg_buf[3] = seg_code_table[cent_sec];
//        // 处理小数点：个位后显示，根据dp_flag控制亮灭
//        if(dp_flag == 1) seg_buf[1] &= 0x7F; // 清bit7，小数点亮
//    }
//}

///**
// * @brief  外部中断下降沿回调，按键按下触发
// * @param  GPIO_Pin: 触发中断的引脚
// * @retval None
// */
//void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
//{
//    static uint32_t last_key_time[3] = {0}; // 按键消抖时间记录
//    uint32_t now = HAL_GetTick();

//    // 200ms软件消抖，避免误触发
//    if(GPIO_Pin == GPIO_PIN_0) // K2：启停切换
//    {
//        if(now - last_key_time[0] > 200)
//        {
//            run_flag = !run_flag;
//            last_key_time[0] = now;
//        }
//    }
//    else if(GPIO_Pin == GPIO_PIN_1) // K3：计时清零
//    {
//        if(now - last_key_time[1] > 200)
//        {
//            cent_sec = 0;
//            dec_sec = 0;
//            sec_unit = 0;
//            sec_tens = 0;
//            ms_count = 0;
//            last_key_time[1] = now;
//        }
//    }
//    else if(GPIO_Pin == GPIO_PIN_2) // K4：上传计时值
//    {
//        if(now - last_key_time[2] > 200)
//        {
//            send_flag = 1;
//            last_key_time[2] = now;
//        }
//    }
//}