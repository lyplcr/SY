#include "GUI_Private.h"
#include "stm32f4xx.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetCharCode
*
* Purpose:
*   Return the UNICODE character code of the current character.
*/
static U16 _GetCharCode(const char GUI_UNI_PTR * s) {
  if((uint8_t)(*s) > 0xA0)
  {
    return *(const U16 GUI_UNI_PTR *)s;
  }
  return *(const U8 GUI_UNI_PTR *)s;
}
/*********************************************************************
*
*       _GetCharSize
*
* Purpose:
*   Return the number of bytes of the current character.
*/
static int _GetCharSize(const char GUI_UNI_PTR * s) {
  GUI_USE_PARA(s);
  if((uint8_t)(*s) > 0xA0)
  {
    return 2;
  }
  return 1;
}
/*********************************************************************
*
*       _CalcSizeOfChar
*
* Purpose:
*   Return the number of bytes needed for the given character.
*/
static int _CalcSizeOfChar(U16 Char) {
  GUI_USE_PARA(Char);
  if(Char > 0xA0A0)
  {
    return 2;
  }
  return 1;
}
/*********************************************************************
*
*       _Encode
*
* Purpose:
*   Encode character into 1/2/3 bytes.
*/
static int _Encode(char *s, U16 Char) {
  if(Char > 0xA0A0)
  {
    *((U16 *)s) = (U16)(Char);
    return 2;
  }
  *s = (U8)(Char);
  return 1;
}
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _API_Table
*/
const GUI_UC_ENC_APILIST GUI__API_TableNone = {
  _GetCharCode,     /*  return character code as U16 */
  _GetCharSize,     /*  return size of character: 1 */
  _CalcSizeOfChar,  /*  return size of character: 1 */
  _Encode           /*  Encode character */
};
const GUI_UC_ENC_APILIST GUI_UC_None = {
  _GetCharCode,     /*  return character code as U16 */
  _GetCharSize,     /*  return size of character: 1 */
  _CalcSizeOfChar,  /*  return size of character: 1 */
  _Encode           /*  Encode character */
};

