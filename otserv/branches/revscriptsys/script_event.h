//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_SCRIPT_EVENT__
#define __OTSERV_SCRIPT_EVENT__

#include <string>
#include "boost/any.hpp"
#include "boost_common.h"

#include "script_enviroment.h"
#include "script_listener.h"

#include "const.h"
#include "thing.h"
#include "creature.h"

// Forward declarations
class Creature;
class PositionEx;

class LuaState;
class LuaThread;
typedef boost::shared_ptr<LuaThread> LuaThread_ptr;

namespace Script {
	class Manager;
	class Enviroment;
}


template<class T, class ScriptInformation>
	bool dispatchEvent(T* e, Script::Manager& state, Script::Enviroment& enviroment, Script::ListenerList& specific_list);
template<class T>
	bool dispatchEvent(T* e, Script::Manager& state, Script::Enviroment& enviroment, Script::ListenerList& specific_list);

namespace Script {

	///////////////////////////////////////////////////////////////////////////////
	// Event template

	class Event {
	public:
		Event();
		virtual ~Event();

		virtual std::string getName() const = 0;

		// Runs the event (ie. triggers all concerned listeners)
		virtual bool dispatch(Manager& state, Script::Enviroment& enviroment) = 0;
		
		// Lua stack manipulation, push
		virtual void push_instance(LuaState& state, Script::Enviroment& enviroment) = 0;
		// update, peek at top table and fill this event with values from it)
		virtual void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread) = 0;
	protected:
		bool call(Manager& stae, Enviroment& enviroment, Listener_ptr listener);

		uint32_t eventID;
		static uint32_t eventID_counter;
		std::string lua_tag;
		bool propagate_by_default;
		
		template<class T, class ScriptInformation> friend
			bool ::dispatchEvent(T* e, 
				Script::Manager& state, 
				Script::Enviroment& enviroment, 
				Script::ListenerList& specific_list);
		
		template<class T> friend
			bool ::dispatchEvent(T* e, 
				Script::Manager& state, 
				Script::Enviroment& enviroment, 
				Script::ListenerList& specific_list);

	};

	///////////////////////////////////////////////////////////////////////////////
	// OnSay event
	// Triggered when a creature talks

	namespace OnSay {
		enum FilterType {
			FILTER_ALL,
			FILTER_SUBSTRING,
			FILTER_MATCH_BEGINNING,
			FILTER_EXACT,
		};

		struct ScriptInformation {
			std::string filter;
			FilterType method;
			bool case_sensitive;
		};

		class Event : public Script::Event {
		public:
			Event(Creature* speaker, SpeakClass& speak_class, ChatChannel* channel, std::string& text);
			~Event();

			std::string getName() const {return "OnSay";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);
			
			// This checks if the script information matches this events prerequiste (data members)
			bool check_match(const ScriptInformation& info);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Creature* speaker;
			SpeakClass& speak_class;
			ChatChannel* channel;
			std::string& text;
		};
	}


	////////////////////////////////////////////////////////////////
	// OnUseItem event
	// Triggered when a player uses an item

	namespace OnUseItem {
		enum FilterType {
			FILTER_ITEMID,
			FILTER_UNIQUEID,
			FILTER_ACTIONID,
		};

		struct ScriptInformation {
			FilterType method;
			uint16_t id;
		};

		class Event : public Script::Event {
		public:
			Event(Player* user, Item* item, const PositionEx* toPos, ReturnValue& retval);
			~Event();

			std::string getName() const {return "OnUseItem";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);
			
			// This checks if the script information matches this events prerequiste (data members)
			bool check_match(const ScriptInformation& info);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* user;
			Item* item;
			const PositionEx* targetPos;
			ReturnValue& retval;
		};
	}

	////////////////////////////////////////////////////////////////
	// OnEquipItem event
	// Triggered when a player equip an item

	namespace OnEquipItem {
		enum FilterType {
			FILTER_ITEMID,
			FILTER_UNIQUEID,
			FILTER_ACTIONID,
		};

		struct ScriptInformation {
			FilterType method;
			uint16_t id;
			uint32_t slot;
			bool equip;
		};

		class Event : public Script::Event {
		public:
			Event(Player* user, Item* item, slots_t slot, bool equip);
			~Event();

			std::string getName() const {return "OnEquipItem";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);
			
			// This checks if the script information matches this events prerequiste (data members)
			bool check_match(const ScriptInformation& info);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* user;
			Item* item;
			uint32_t equipslot;
			bool equip;
		};
	}

	////////////////////////////////////////////////////////////////
	// OnMoveCreature event
	// Triggered when a creature moves

	namespace OnMoveCreature {
		enum FilterType {
			FILTER_ITEMID,
			FILTER_UNIQUEID,
			FILTER_ACTIONID,
		};

		struct ScriptInformation {
			FilterType method;
			uint16_t id;
			bool stepIn;
		};

		class Event : public Script::Event {
		public:
			Event(Creature* creature, Tile* tile, bool stepIn);
			~Event();

			std::string getName() const {return "OnMoveCreature";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);
			
			// This checks if the script information matches this events prerequiste (data members)
			bool check_match(const ScriptInformation& info);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Creature* creature;
			Tile* tile;
			bool stepIn;
		};
	}

	////////////////////////////////////////////////////////////////
	// OnMoveItem event
	// Triggered when an item is moved

	namespace OnMoveItem {
		enum FilterType {
			FILTER_ITEMID,
			FILTER_UNIQUEID,
			FILTER_ACTIONID,
		};

		struct ScriptInformation {
			FilterType method;
			uint16_t id;
			bool addItem;
			bool isItemOnTile;
		};

		class Event : public Script::Event {
		public:
			Event(Item* item, Tile* tile, bool addItem);
			~Event();

			std::string getName() const {return "OnMoveItem";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);
			
			// This checks if the script information matches this events prerequiste (data members)
			bool check_match(const ScriptInformation& info);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Item* item;
			Tile* tile;
			bool addItem;
		};
	}


	///////////////////////////////////////////////////////////////////////////////
	// OnJoinChannel event
	// Triggered when a player opens a new chat channel

	namespace OnJoinChannel {
		class Event : public Script::Event {
		public:
			Event(Player* chatter, ChatChannel* chat);
			~Event();

			std::string getName() const {return "OnJoinChannel";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* chatter;
			ChatChannel* channel;
		};
	}

	///////////////////////////////////////////////////////////////////////////////
	// OnLeaveChannel event
	// Triggered when a player closes an existing chat channel

	namespace OnLeaveChannel {
		class Event : public Script::Event {
		public:
			Event(Player* chatter, ChatChannel* chat);
			~Event();

			std::string getName() const {return "OnLeaveChannel";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* chatter;
			ChatChannel* channel;
		};
	}

	///////////////////////////////////////////////////////////////////////////////
	// OnLogin event
	// Triggered when a player enters the server

	namespace OnLogin {
		class Event : public Script::Event {
		public:
			Event(Player* player);
			~Event();

			std::string getName() const {return "OnLogin";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* player;
		};
	}

	///////////////////////////////////////////////////////////////////////////////
	// OnLogout event
	// Triggered when a player leaves the server

	namespace OnLogout {
		class Event : public Script::Event {
		public:
			Event(Player* player, bool forced, bool timeout);
			~Event();

			std::string getName() const {return "OnLogout";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* player;
			bool forced;
			bool timeout;
		};
	}

	///////////////////////////////////////////////////////////////////////////////
	// OnLook event
	// Triggered when a looks at an object

	namespace OnLook {
		enum FilterType {
			FILTER_NONE,
			FILTER_ITEMID,
			FILTER_UNIQUEID,
			FILTER_ACTIONID,
			FILTER_CREATUREID,
		};

		struct ScriptInformation {
			FilterType method;
			uint32_t id;
		};

		class Event : public Script::Event {
		public:
			Event(Player* player, std::string& desc, Thing* object);
			~Event();

			std::string getName() const {return "OnLook";}
			
			// Runs the event
			bool dispatch(Manager& state, Enviroment& enviroment);
			
			// This checks if the script information matches this events prerequiste (data members)
			bool check_match(const ScriptInformation& info);

			// Lua stack manipulation
			void push_instance(LuaState& state, Enviroment& enviroment);
			void update_instance(Manager& state, Script::Enviroment& enviroment, LuaThread_ptr thread);

		protected:
			Player* player;
			std::string& desc;
			Thing* object;
		};
	}
	
	/* To add a new event
	 * 1. Create the event class
	 * 2. Expose a registerListener function to lua
	 * 3. Add the class to Enviroment::stopListener
	 * 4. Add callback from an arbitrary location in otserv source
	 */
}

///////////////////////////////////////////////////////////////////////////////
// Implementation details

template<class T, class ScriptInformation>
bool dispatchEvent(T* e, Script::Manager& state, Script::Enviroment& enviroment, Script::ListenerList& specific_list) {
	if(specific_list.size() == 0) {
		return false;
	}
	for(Script::ListenerList::iterator event_iter = specific_list.begin();
		event_iter != specific_list.end();
		++event_iter)
	{
		Listener_ptr listener = *event_iter;
		if(listener->isActive() == false) continue;
		const ScriptInformation& info = boost::any_cast<const ScriptInformation>(listener->getData());

		// Call handler
		if(e->check_match(info)) {
			if(e->call(state, enviroment, listener) == true) {
				// Handled
				return true;
			}
		}
	}
	return false;
}

template<class T> // No script information!
bool dispatchEvent(T* e, Script::Manager& state, Script::Enviroment& enviroment, Script::ListenerList& specific_list) {
	if(specific_list.size() == 0) {
		return false;
	}
	for(Script::ListenerList::iterator event_iter = specific_list.begin();
		event_iter != specific_list.end();
		++event_iter)
	{
		Listener_ptr listener = *event_iter;
		if(listener->isActive() == false) continue;

		// Call handler
		if(e->call(state, enviroment, listener) == true) {
			// Handled
			return true;
		}
	}
	return false;
}

#endif // __OTSERV_SCRIPT_EVENT__
