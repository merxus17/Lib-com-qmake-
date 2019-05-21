#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include "../Events/EventConstants.h"
#include <QEvent>
/**
 * Esta classe deve servir de base para todos os eventos especializados e tem a funcao de receber um tipo de evento e
 * construir o nosso evento especializado correspondente.Recebe o evento especefico atraves do enum e cria o evento
 * customizado e retorna ele.
 */
namespace rm
{
    class GraphicsSceneEvent
    {
    public:

        /**
         * Retorna um graphics scene event baseado no enum passado para a funcaoo , com todos os dados necessarios
         * retirados do qevent
         */
         GraphicsSceneEvent(EventType eventType);

         /**
         * Default destructor
         */
         ~GraphicsSceneEvent() = default;

         /**
          * @brief convertMouseButtons - convert from qt mouse buttons to rm mouse buttons.
          * @param qtButtons -  integer with qt mouse buttons.
          * @return - integer with rm mouse buttons.
          */
         MouseButtons convertMouseButtons(unsigned int qtButtons) const;

         /**
          * @brief convertKeyboardModifiers - convert from qt modifiers to rm modifiers.
          * @param qtModifiers - Integer with qt modifiers.
          * @return - Integer with rm modifiers.
          */
         KeyboardModifiers convertKeyboardModifiers(unsigned int qtModifiers) const;

         /**
          * @brief getEventType - Return the current event type represented for this object.
          * @return - The current event type represented for this object.
          */
         EventType getEventType();
    private:

         /**
         * @brief eventType Enumeration  with all event types available to be received from Qt.
         *
         */
        EventType _eventType;
    };
}
