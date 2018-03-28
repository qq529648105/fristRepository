/****************************************************************************
**
**  Created By:  Daiyl  at  2014-2-19
**  Copyright (c) 2014 Dzics.com, All rights reserved.
**
****************************************************************************/

#include "AxisObject.h"

AxisObject::AxisObject(QObject *parent) :
    QObject(parent)
{
    this->value = 0;
}

AxisObject::~AxisObject()
{
}
