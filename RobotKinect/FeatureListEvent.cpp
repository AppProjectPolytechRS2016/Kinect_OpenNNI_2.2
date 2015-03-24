/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "FeatureListEvent.h"

FeatureListEvent::FeatureListEvent(EventSource* source, std::vector<std::string>featureList){
    this->source = source;
    this->featureList = featureList;
}

std::vector<std::string> FeatureListEvent::getFeatureList(){
    return featureList;
}