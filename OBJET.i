/* file: OBJET.i */
%module OBJET

%{

#include <vector>
#include "OBJET.h"

%}

%include "std_string.i"
%include "std_vector.i"
%include "typemaps.i"

namespace std
{
   %template(IntVector) vector<int>;
   %template(FloatVector) vector<float>;
}


%include "OBJET.h"
