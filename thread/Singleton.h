#ifndef __SINGLETON__
#define __SINGLETON__
template<typename T>
class Singleton
{
private:
    /* data */
    static T** GetPPtr()
    {
        static T* ptr = nullptr;
        return &ptr;
    }
public:
    static T* Get()
    {
        return *GetPPtr();
    }

    template<typename... Args>
    static void New(Args&&... args)
    {
        *GetPPtr() = new T(std::forward<Args>(args)...);
    }

    static void Delete()
    {
        if (Get()!=nullptr)
        {
            delete Get();
            *GetPPtr() = nullptr;
        }
    }
};


#endif