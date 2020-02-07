//=========================================================================================================================!
// Implementation base on talk from Stephan T Lavavej
// https://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-3-of-n?ocid=player
//=========================================================================================================================!

// Vector like containers
#include <vector>
#include <deque>

// List like containers
#include <list>
#include <forward_list>

// Associative containers
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

// For the actual removal
#include <algorithm>

//=========================================================================================================================!
namespace av {namespace utils {
//=========================================================================================================================!

namespace detail 
{
	// Empty structs defined because we needed types
	// to differentiate between different kinds of containers
	struct vector_like_tag {};
	struct list_like_tag {};
	struct associative_tag {};

	template <typename Container> struct container_traits;

	// Partial specializations on each type of container so as to specify the correct category for it
	template <typename ValueType, typename AllocatorType>
	struct container_traits<std::vector<ValueType, AllocatorType>>
	{
		using category = vector_like_tag;
	};

	template <typename ValueType, typename AllocatorType>
	struct container_traits<std::deque<ValueType, AllocatorType>>
	{
		using category = vector_like_tag;
	};

	template <typename ValueType, typename AllocatorType>
	struct container_traits<std::list<ValueType, AllocatorType>>
	{
		using category = list_like_tag;
	};

	template <typename ValueType, typename AllocatorType>
	struct container_traits<std::forward_list<ValueType, AllocatorType>>
	{
		using category = list_like_tag;
	};

	template <typename ValueType, typename PredicateType, typename AllocatorType>
	struct container_traits<std::set<ValueType, PredicateType, AllocatorType>>
	{
		using category = associative_tag;
	};

	template <typename ValueType, typename PredicateType, typename AllocatorType>
	struct container_traits<std::unordered_set<ValueType, PredicateType, AllocatorType>>
	{
		using category = associative_tag;
	};

	template <typename KeyType, typename ValueType, typename PredicateType, typename AllocatorType>
	struct container_traits<std::map<KeyType, ValueType, PredicateType, AllocatorType>>
	{
		using category = associative_tag;
	};

	template <typename KeyType, typename ValueType, typename PredicateType, typename AllocatorType>
	struct container_traits<std::unordered_map<KeyType, ValueType, PredicateType, AllocatorType>>
	{
		using category = associative_tag;
	};

	// Erase internal implementations 

	// For vector like containers
	template <typename ContainerType, typename ValueType>
	void erase_helper(ContainerType& cont, const ValueType& val, vector_like_tag)
	{
		cont.erase(std::remove(cont.begin(), cont.end(), val), cont.end());
	}

	template <typename ContainerType, typename PredType>
	void erase_if_helper(ContainerType& cont, PredType pred, vector_like_tag)
	{
		cont.erase(std::remove_if(cont.begin(), cont.end(), pred), cont.end());
	}

	// For list like containers
	template <typename ContainerType, typename ValueType>
	void erase_helper(ContainerType& cont, const ValueType& val, list_like_tag)
	{
		cont.remove(val);
	}

	template <typename ContainerType, typename PredType>
	void erase_if_helper(ContainerType& cont, PredType pred, list_like_tag)
	{
		cont.remove_if(std::remove_if(cont.begin(), cont.end(), pred), cont.end());
	}

	// For associative containers
	template <typename ContainerType, typename ValueType>
	void erase_helper(ContainerType& cont, ValueType val, associative_tag)
	{
		cont.erase(val);
	}

	template <typename ContainerType, typename PredType>
	void erase_if_helper(ContainerType& cont, PredType pred, associative_tag)
	{
		for (auto itr = cont.begin(); itr!= cont.end();)
		{
			if (pred(itr))
			{
				cont.erase(itr++);	// Move onto the next iterator before removal of old value
									// as the removal will invalidate the iterator
			}
			else
			{
				++itr;
			}
		}
	}
}

//=========================================================================================================================!

// Erase implementations to be used by clients
// NOTES:
// 1.	Because we are referring to a nested typedef (something that is dependent on a template parameter), 
//		we need to say typename to disambiguate the compiler.
// 2.	We need an object of the above type for overloading, so we create a temporary. Compiler should
//		ideally optimize it away as we are not using this temporary in any way apart from for overload resolution
template <typename Container, typename ValueType>
void erase(Container& cont, const ValueType& val)
{
	detail::erase_helper(cont, val, typename detail::container_traits<Container>::category());
}

template <typename Container, typename PredType>
void erase_if(Container& cont, PredType pred)
{
	detail::erase_if_helper(cont, pred, typename detail::container_traits<Container>::category());
}

//=========================================================================================================================!
}}
//=========================================================================================================================!
