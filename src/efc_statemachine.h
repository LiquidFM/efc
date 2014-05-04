/** @file efc_statemachine.h
 *  @brief TODO: Put description here.
 *
 * TODO: Put description here.
 *
 * @copyright
 * This file is part of the EFC library.
 * @n@n
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 * @n@n
 * the EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * @n@n
 * the EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * @n@n
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EFC_STATEMACHINE_H_
#define EFC_STATEMACHINE_H_

#include <cstddef>
#include <efc/Typelist.h>


#define RULE(FromState, Event, ToState, Context, ContextMethod) \
	::EFC::Rule<Context>::Definition<FromState, Event, ToState, &Context::ContextMethod>


namespace EFC {

template <typename Context>
struct Rule
{
	typedef void (Context::*Action)(const char *input, int size);

	template <int FromState, int Token, int ToState, Action action>
	struct Definition
	{
		typedef Context rule_context;

		enum
        {
        	FROM_STATE = FromState,
        	TOKEN = Token,
        	TO_STATE = ToState
        };

		static inline void invoke(Context *context, const char *input, int size)
		{
			(context->*action)(input, size);
		}
	};
};


struct Data
{
	void *context;
	int state;

	const char *iterator;
	const char *input;
};


template <typename Context, typename RulesTypeList>
struct ProcessEvent
{
	static inline void process(Data &data)
	{
		typedef typename RulesTypeList::Head RulesTypeList_Item;

		if (RulesTypeList_Item::FROM_STATE == data.state && RulesTypeList_Item::TOKEN == *data.iterator)
		{
			data.state = RulesTypeList_Item::TO_STATE;
			RulesTypeList_Item::invoke(static_cast<typename RulesTypeList_Item::rule_context *>(data.context), data.input, data.iterator - data.input);
			data.input = data.iterator + 1;
			return;
		}

		ProcessEvent<Context, typename RulesTypeList::Tail>::process(data);
	}
};

template <typename Context>
struct ProcessEvent<Context, ::Loki::NullType>
{
	static inline void process(Data &data)
	{}
};


template <typename RulesTypeList>
class StateMachine
{
public:
	typedef typename RulesTypeList::Head::rule_context Context;

public:
	StateMachine(Context &context)
	{
		m_data.context = &context;
		m_data.state = RulesTypeList::Head::FROM_STATE;
		m_data.iterator = NULL;
		m_data.input = NULL;
	}

	StateMachine(Context *context)
	{
		m_data.context = context;
		m_data.state = RulesTypeList::Head::FROM_STATE;
		m_data.iterator = NULL;
		m_data.input = NULL;
	}

	inline int process(const char *string)
	{
		for (m_data.input = m_data.iterator = string; *m_data.iterator; ++m_data.iterator)
			ProcessEvent<Context, RulesTypeList>::process(m_data);

		ProcessEvent<Context, RulesTypeList>::process(m_data);

		return m_data.state;
	}

protected:
	Data m_data;
};

}

#endif /* EFC_STATEMACHINE_H_ */
