#ifndef GIE_EVENTVECTOR_H
#define GIE_EVENTVECTOR_H

#include <vector>
#include <variant>

template<typename... EventTypes>
struct EventVector
{
    struct Event
    {
        std::variant<EventTypes...> event;
        bool deleted;
    };

    std::vector<Event> events;

    struct Iterator
    {
        struct DeletableEvent
        {
            int i;
            std::vector<Event>* v;

            std::variant<EventTypes...>& operator*()
            {
                return (*v)[i].event;
            }

            void tag_deleted()
            {
                (*v)[i].deleted = true;
            }
        };

        std::vector<Event>* v;
        int i = -1;

        Iterator& operator++()
        {
            if(i != -1)
            {
                i ++;
                while(i < static_cast<int>(v->size()) && (*v)[i].deleted)
                    i ++;
                if(i == static_cast<int>(v->size()))
                    i = -1;
            }

            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return i != other.i;
        }

        auto operator*()
        {
            return DeletableEvent{.i = i, .v = v};
        }
    };

    struct ConstIterator
    {
        const std::vector<Event>* v;
        int i = -1;

        ConstIterator& operator++()
        {
            if(i != -1)
            {
                i ++;
                while(i < static_cast<int>(v->size()) && (*v)[i].deleted)
                    i ++;
                if(i == static_cast<int>(v->size()))
                    i = -1;
            }

            return *this;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return i != other.i;
        }

        const auto& operator*()
        {
            return (*v)[i].event;
        }
    };

    void clear()
    {
        events.clear();
    }

    template<typename T>
    void add(T&& t)
    {
        events.template emplace_back(Event {
                .event = std::forward<T>(t),
                .deleted = false
        });
    }

    [[nodiscard]] Iterator begin()
    {
        int i = 0;
        while(i < static_cast<int>(events.size()) && events[i].deleted)
            i++;

        return Iterator {
                .v = &events,
                .i = i != static_cast<int>(events.size()) ? i : -1
        };
    }

    [[nodiscard]] Iterator end()
    {
        return Iterator{};
    }

    [[nodiscard]] ConstIterator begin() const
    {
        int i = 0;
        while(i < static_cast<int>(events.size()) && events[i].deleted)
            i++;

        return ConstIterator {
                .v = &events,
                .i = i != static_cast<int>(events.size()) ? i : -1
        };
    }

    [[nodiscard]] ConstIterator end() const
    {
        return ConstIterator{};
    }
};

template<typename Event, typename... EventTypes>
struct FilterEvent
{
    EventVector<EventTypes...>* events;

    struct Iterator
    {
        typename EventVector<EventTypes...>::Iterator iterator;

        struct DeletableEvent
        {
            int i;
            decltype(EventVector<EventTypes...>::events)* v;

            Event& operator*()
            {
                return *std::get_if<Event>(&(*v)[i].event);
            }

            Event* operator->()
            {
                return std::get_if<Event>(&(*v)[i].event);
            }

            void tag_deleted()
            {
                (*v)[i].deleted = true;
            }
        };

        Iterator& operator++()
        {
            ++iterator;
            while(iterator != typename EventVector<EventTypes...>::Iterator{} && !std::get_if<Event>(&(*(*iterator))))
                ++iterator;

            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return iterator != other.iterator;
        }

        auto operator*()
        {
            return DeletableEvent{.i = iterator.i, .v = iterator.v};
        }
    };

    [[nodiscard]] Iterator begin()
    {
        typename EventVector<EventTypes...>::Iterator it = events->begin();

        while(it != typename EventVector<EventTypes...>::Iterator {} && !std::get_if<Event>(&(*(*it))))
            ++it;

        return Iterator{it};
    }

    [[nodiscard]] Iterator end()
    {
        return Iterator{events->end()};
    }
};

template<typename Event, typename... EventTypes>
struct ConstFilterEvent
{
    const EventVector<EventTypes...>* events;

    struct Iterator
    {
        typename EventVector<EventTypes...>::ConstIterator iterator;

        Iterator& operator++()
        {
            ++iterator;
            while(iterator != typename EventVector<EventTypes...>::ConstIterator{} && !std::get_if<Event>(&(*iterator)))
                ++iterator;

            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return iterator != other.iterator;
        }

        const auto& operator*()
        {
            return *std::get_if<Event>(&(*iterator));
        }
    };

    [[nodiscard]] Iterator begin()
    {
        typename EventVector<EventTypes...>::ConstIterator it = events->begin();

        while(it != typename EventVector<EventTypes...>::ConstIterator{} && !std::get_if<Event>(&(*it)))
            ++it;

        return Iterator{it};
    }

    [[nodiscard]] Iterator end()
    {
        return Iterator{events->end()};
    }
};

template <typename Event, typename... EventTypes>
FilterEvent<Event, EventTypes...> filter_events(EventVector<EventTypes...>& events)
{
    return FilterEvent<Event, EventTypes...>{&events};
}

template <typename Event, typename... EventTypes>
ConstFilterEvent<Event, EventTypes...> filter_events(const EventVector<EventTypes...>& events)
{
    return ConstFilterEvent<Event, EventTypes...>{&events};
}

#endif //GIE_EVENTVECTOR_H
