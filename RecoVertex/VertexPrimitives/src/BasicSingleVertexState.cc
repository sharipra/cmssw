#include "RecoVertex/VertexPrimitives/interface/BasicSingleVertexState.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"

BasicSingleVertexState::BasicSingleVertexState()
  : thePos(GlobalPoint(0, 0, 0)), thePosAvailable(false),
    theErr(AlgebraicSymMatrix(3, 0)), theErrAvailable(false),
    theWeight(AlgebraicSymMatrix(3, 0)), theWeightAvailable(false),
    theWeightTimesPos(3, 0), theWeightTimesPosAvailable(false),
    theWeightInMix(0.)
{}


BasicSingleVertexState::BasicSingleVertexState(const GlobalPoint & pos,
			     const GlobalError & posErr,
			     const double & weightInMix)
  : thePos(pos), thePosAvailable(true),
    theErr(posErr), theErrAvailable(true),
    theWeight(AlgebraicSymMatrix(3, 0)), theWeightAvailable(false),
    theWeightTimesPos(3, 0), theWeightTimesPosAvailable(false),
    theWeightInMix(weightInMix)
{}


BasicSingleVertexState::BasicSingleVertexState(const GlobalPoint & pos,
			     const GlobalWeight & posWeight,
			     const double & weightInMix)
  : thePos(pos), thePosAvailable(true),
    theErr(AlgebraicSymMatrix(3, 0)), theErrAvailable(false),
    theWeight(posWeight), theWeightAvailable(true),
    theWeightTimesPos(3, 0), theWeightTimesPosAvailable(false),
    theWeightInMix(weightInMix)
{}


BasicSingleVertexState::BasicSingleVertexState(const AlgebraicVector & weightTimesPosition,
			     const GlobalWeight & posWeight,
			     const double & weightInMix)
  : thePos(GlobalPoint(0, 0, 0)), thePosAvailable(false),
    theErr(AlgebraicSymMatrix(3, 0)), theErrAvailable(false),
    theWeight(posWeight), theWeightAvailable(true),
    theWeightTimesPos(weightTimesPosition), theWeightTimesPosAvailable(true),
    theWeightInMix(weightInMix)
{//cout <<"BasicSingleVertexState ctor\n";
}

GlobalPoint BasicSingleVertexState::position() const
{
  if (!thePosAvailable) computePosition();
  return thePos;
}


GlobalError BasicSingleVertexState::error() const
{
  if (!theErrAvailable) computeError();
  return theErr;
}


GlobalWeight BasicSingleVertexState::weight() const
{
  if (!theWeightAvailable) computeWeight();
  return theWeight;
}


AlgebraicVector BasicSingleVertexState::weightTimesPosition() const
{
  if (!theWeightTimesPosAvailable) computeWeightTimesPos();
  return theWeightTimesPos;
}


// RefCountedVertexSeed BasicSingleVertexState::seedWithoutTracks() const
// {
//   RefCountedVertexSeed v = new VertexSeed(position(), error());
//   return v;
// }



void BasicSingleVertexState::computePosition() const
{
  AlgebraicVector pos = error().matrix()*weightTimesPosition();
  thePos = GlobalPoint(pos[0], pos[1], pos[2]);
  thePosAvailable = true;
}


void BasicSingleVertexState::computeError() const
{
  int ifail;
  theErr = weight().matrix().inverse(ifail);
  if (ifail != 0) throw VertexException("BasicSingleVertexState::could not invert weight matrix");
  theErrAvailable = true;
}


void BasicSingleVertexState::computeWeight() const
{
  int ifail;
  theWeight = error().matrix().inverse(ifail);
  if (ifail != 0) throw VertexException("BasicSingleVertexState::could not invert error matrix");
  theWeightAvailable = true;
}


void BasicSingleVertexState::computeWeightTimesPos() const
{
  AlgebraicVector pos(3); pos[0] = position().x();
  pos[1] = position().y(); pos[2] = position().z();
  theWeightTimesPos = weight().matrix()*pos;
  theWeightTimesPosAvailable = true;
}


