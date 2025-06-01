/**
 * @file elab_utils.h
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _ELAB_UTILS_H
#define _ELAB_UTILS_H

//指定宽度向上对齐
#define ELAB_SIZE_ALIGN(size, align) (((size) + (align - 1)) & ~(align - 1))


//指定宽度向下对齐
#define ELAB_SIZE_ALIGN_DOWN(size, align) ((size) & ~(align - 1))
#endif
