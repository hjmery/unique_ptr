/*
--------------------------------------------------------------
Templated unique_ptr class								   |X|
1 template parameter-The type we're pointing to			   |X|
Need deleted methods to keep it from being copied		   |X|
Overload operators member/non-member					   ||
Allocates memory with new								   |X|
Deletes when goes out of scope							   |X|
														   
Constructors											   
-Default												   |X|
-Overloaded that takes a T* and stores it				   |X|
-move													   |X|
Destructor												   |X|
Operators												   
-move assignment =										   |X|
-pointer operator ->									   ||
-dereference operator *									   ||
-equality ==											   ||
-inequality !=											   ||
Others													   
-get, returns a pointer,const							   |X|
-release, releases ownership of pointer, returns it		   |X|
--------------------------------------------------------------
*/
#pragma once

namespace usu
{
    template <typename T>
    class unique_ptr
    {
      public:
        //default initialized the ptr to nullptr
        unique_ptr() :
            m_ptr(nullptr)
        {
            //I couldn't figure out what was wrong but the new wasn't working
            //m_ptr = new T;
        }

        //overloaded
        unique_ptr(T* ptr)
        {
            //I couldn't figure out what was wrong but the new wasn't working
            m_ptr = ptr;
        }

		//destructor
        ~unique_ptr() { delete m_ptr; }

        //move constructor
        unique_ptr(unique_ptr&& ptr) noexcept
        {
            delete m_ptr;
            m_ptr = ptr.m_ptr;
            delete ptr.m_ptr;
        }

        //move assignment
        unique_ptr& operator=(unique_ptr&& ptr)
        {
            delete m_ptr;
            m_ptr = ptr.m_ptr;
            delete ptr.m_ptr;
            return *this;
        }

        //pointer operator
        T* operator->()
        {
            return m_ptr;
        }

        //dereference operator
        T operator*()
        {
            return *m_ptr;
        }

        //equality operator
        bool operator==(unique_ptr& comp)
        {
            return (m_ptr == comp.m_ptr);
        }

        //inequality operator
        bool operator!=(unique_ptr& comp)
        {
            return !(m_ptr == comp.m_ptr);
        }

        //get function that returns a pointer to the managed raw pointer
        const T* get()
        {
            return m_ptr;
        }

        T* release()
        {
            T* copy = m_ptr;
            delete m_ptr;
            return copy;
        }

        //copy constructor and assignment = delete
        unique_ptr(const unique_ptr& ptr) = delete;
        unique_ptr<T>& operator=(const unique_ptr& ptr) = delete;

      private:
        T* m_ptr;
    };

    template <typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
} // namespace usu