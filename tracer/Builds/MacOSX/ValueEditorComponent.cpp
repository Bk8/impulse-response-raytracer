//
//  ValueEditorComponent.cpp
//  tracer
//
//  Created by Reuben Thomas on 02/02/2014.
//
//

#include "ValueEditorComponent.h"

template<>
void ValueEditor<double>::setValue (const String & in, NotificationType nt)
{
    setValue (in.getDoubleValue(), nt);
}

template<>
void ValueEditor<int>::setValue (const String & in, NotificationType nt)
{
    setValue (in.getIntValue(), nt);
}
