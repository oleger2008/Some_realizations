template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value_(initial)
    {
    }

    struct Access {
        T& ref_to_value;
        mutex* value_mutex_ptr;

        Access(T& ref, mutex* value_mutex)
        : ref_to_value(ref)
        , value_mutex_ptr(value_mutex)
		{
        	value_mutex_ptr->lock();
		}

        ~Access() {
        	value_mutex_ptr->unlock();
        }
    };

    Access GetAccess() {
    	Access access(value_, &value_mutex_);
    	return access;
    }
private:
    T value_;
    mutex value_mutex_;
};