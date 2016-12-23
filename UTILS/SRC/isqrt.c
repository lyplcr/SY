//*****************************************************************************
//
// isqrt.c - Integer square root.
//
// Copyright (c) 2005-2009 Luminary Micro, Inc.  All rights reserved.
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 4423 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#include "utils.h"

//*****************************************************************************
//
//! \addtogroup isqrt_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Compute the integer square root of an integer.
//!
//! \param ulValue is the value whose square root is desired.
//!
//! This function will compute the integer square root of the given input
//! value.  Since the value returned is also an integer, it is actually better
//! defined as the largest integer whose square is less than or equal to the
//! input value.
//!
//! \return Returns the square root of the input value.
//
//*****************************************************************************
unsigned long
isqrt(unsigned long ulValue)
{
    unsigned long ulRem, ulRoot, ulIdx;

    //
    // Initialize the remainder and root to zero.
    //
    ulRem = 0;
    ulRoot = 0;

    //
    // Loop over the sixteen bits in the root.
    //
    for(ulIdx = 0; ulIdx < 16; ulIdx++)
    {
        //
        // Shift the root up by a bit to make room for the new bit that is
        // about to be computed.
        //
        ulRoot <<= 1;

        //
        // Get two more bits from the input into the remainder.
        //
        ulRem = ((ulRem << 2) + (ulValue >> 30));
        ulValue <<= 2;

        //
        // Make the test root be 2n + 1.
        //
        ulRoot++;

        //
        // See if the root is greater than the remainder.
        //
        if(ulRoot <= ulRem)
        {
            //
            // Subtract the test root from the remainder.
            //
            ulRem -= ulRoot;

            //
            // Increment the root, setting the second LSB.
            //
            ulRoot++;
        }
        else
        {
            //
            // The root is greater than the remainder, so the new bit of the
            // root is actually zero.
            //
            ulRoot--;
        }
    }

    //
    // Return the computed root.
    //
    return(ulRoot >> 1);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
