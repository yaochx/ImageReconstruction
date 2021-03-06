/*=========================================================================
 *
 *  Copyright Kitware Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __itkMaxPhaseCorrelationOptimizer_txx
#define __itkMaxPhaseCorrelationOptimizer_txx

#include "itkMaxPhaseCorrelationOptimizer.h"

/*
 * \author Jakub Bican, jakub.bican@matfyz.cz, Department of Image Processing,
 *         Institute of Information Theory and Automation,
 *         Academy of Sciences of the Czech Republic.
 *
 */

namespace itk
{

/*
 * Constructor
 */
template < typename TRegistrationMethod >
MaxPhaseCorrelationOptimizer<TRegistrationMethod>
::MaxPhaseCorrelationOptimizer() : Superclass()
{
  m_MaxCalculator = MaxCalculatorType::New();
}


/**
 *
 */
template < typename TRegistrationMethod >
void
MaxPhaseCorrelationOptimizer<TRegistrationMethod>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "MaxCalculator: " << m_MaxCalculator << std::endl;
}


/**
 *
 */
template < typename TRegistrationMethod >
void
MaxPhaseCorrelationOptimizer<TRegistrationMethod>
::ComputeOffset()
{
  ImageConstPointer input = static_cast< ImageType * >(this->GetInput(0));

  OffsetType offset;
  offset.Fill( 0 );

  if (!input)
    return;

  m_MaxCalculator->SetImage( input );

  try
    {
    m_MaxCalculator->ComputeMaximum();
    }
  catch( ExceptionObject& err )
    {
    itkDebugMacro( "exception caught during execution of MaxCalculatior - passing " );
    throw err;
    }

  typename ImageType::IndexType
              index   = m_MaxCalculator->GetIndexOfMaximum();
  typename ImageType::SizeType
              size    = input->GetLargestPossibleRegion().GetSize();
  typename ImageType::SpacingType
              spacing = input->GetSpacing();

  for (int i = 0; i < ImageDimension ; i++)
    {
    if ( index[i] > vcl_floor( size[i] / 2.0 ) )
      {
      offset[i] = -1*(index[i] - size[i]) * spacing[i];
      }
    else
      {
      offset[i] = -1*index[i] * spacing[i];
      }
    }

  this->SetOffset( offset );
}

/**
 *
 */
template < typename TRegistrationMethod >
unsigned long
MaxPhaseCorrelationOptimizer<TRegistrationMethod>
::GetMTime() const
{
  unsigned long mtime = Superclass::GetMTime();
  unsigned long m;

  if (m_MaxCalculator)
    {
    m = m_MaxCalculator->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  return mtime;
}

} //end namespace itk


#endif
