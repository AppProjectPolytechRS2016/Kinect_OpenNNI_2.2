/*******************************************************************************
 *                                                                              *
 *   APP RS 2016 - ApplicationKinect                                            *
 *   Polytech Annecy Chambery                                                   *
 *                                                                              *
 *******************************************************************************/

#include "OrderAcceptedEvent.h"

OrderAcceptedEvent::OrderAcceptedEvent(EventSource* source, bool accepted){
    this->accepted = accepted;
    this->source = source;
}
bool OrderAcceptedEvent::getAccepted(){
    return accepted;
}