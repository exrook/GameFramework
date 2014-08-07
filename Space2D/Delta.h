#ifndef _DELTA_H_
#define _DELTA_H_

#include "Declarations.h"
#include "ControlInterfaces/Loopable.h"

namespace jer
{

    template<class T>
    class Delta: public T, public Loopable
    /** Class representing derivatives of certain scalars/vectors.
     * The rate of change itself is of the same type (ie, an increment)
     * Class will have a function for a change across t time-units which is up to the user
     * Will also have a function for an increment across 1 time-unit (iterations maybe) for ease
     * Someone may want the velocity of a point as so
     * <code>Delta\<Vector\> velocity</code>
     * they could get the position using the current() function
     * Someone may want the acceleration of a point as so
     * <code>Delta\<Delta\<Vector\> \> accleration</code>
     * they could get the velocity using the current(1) function where 1 represents how many integrals you take
     * they could get the current position using current(2) OR current(). 
     * current() will be specialized to recursively call current for Delta\<Delta\<T\> \>
     */
    {
	public:
		typedef T BASE_TYPE;
		enum {N_DERIVATIVE=1};
		
    private:
        T value;
        
    public:
        Delta(const T &increment, const T &init): T(increment), value(init) {};
        Delta(const T &increment): T(increment), value(){};
        Delta(const Delta<T> &other): T(other), value(other.value) {};
        Delta(): T(), value() {};
        
    public:
        const BASE_TYPE &get(int i=1) const {return value;};
        void set(const T& val) {value = val;};
        void increment(const double iterations=1.0) {value += iterations*(*this);};
        const SUCCESS loop() override {increment(); return SUCCEEDED;};
    };
    
    template<class T>
    class Delta<Delta<T> >: public Delta<T>::BASE_TYPE, public Loopable
    {
	public:
		typedef typename Delta<T>::BASE_TYPE BASE_TYPE;
		enum {N_DERIVATIVE=Delta<T>::N_DERIVATIVE+1};
	
    private:
        Delta<T> value;
        BASE_TYPE *delta;
        
    public:
        Delta(const Delta<T> &increment, const Delta<T> &init): BASE_TYPE(increment), value(init), delta(this) {};
        explicit Delta(const Delta<T> &increment): BASE_TYPE(increment), value(), delta(this) {};
        Delta(): BASE_TYPE(), value(), delta(this) {};
        
    public:
        const BASE_TYPE &get(int i=1) const {if(i == N_DERIVATIVE) return value; else return value.get(i);};
        void set(const Delta<T>& val) {value = val;};
		void increment(const double iterations=1.0) {value += iterations*(*delta); value.increment();};
        const SUCCESS loop() override {increment(); return SUCCEEDED;};
    };
    
    template<class T>
    class Delta<T *>: public T, public Loopable
    {
    public:
        typedef T BASE_TYPE;
        enum {N_DERIVATIVE=1};
        
    private:
        T *value;
        bool has;
        
    public:
        Delta(const T &increment, const T &init): T(increment), value(new T(init)), has(true) {};
        Delta(const T &increment): T(increment), value(NULL), has(false) {};
        Delta(const T &increment, T * const val): T(increment), value(val), has(false) {};
        Delta(T * const val): T(), value(val), has(false) {};
        Delta(const Delta<T *> &other, bool give=false): T(other), value(other.value), has(give) {};
        Delta(): T(), value(NULL), has(false) {};
        virtual ~Delta() {if(has) delete value;};
        
    public:
        void set(const T& val)
        {
            if(this->value == NULL)
            {
                this->value = new T(val);
                has = true;
            }
            else
            {
                *(this->value) = val;
            }
        }
        
        void set(T * const valPtr)
        {
            if(this->value && has)
            {
                delete this->value;
            }
            
            this->value = valPtr;
            has = false;
        }
        
    public:
        const T &get(int i=1) const {return *value;};
        void increment(const double iterations=1.0) {value += iterations*(*this);};
        const SUCCESS loop() {increment(); return SUCCEEDED;};
        //void increment() {increment(1.0);};
    };
    
    template<class T>
    class Delta<Delta<T> *>: public Delta<T>::BASE_TYPE, public Loopable
    {
    public:
        typedef typename Delta<T>::BASE_TYPE BASE_TYPE;
        enum {N_DERIVATIVE=Delta<T>::N_DERIVATIVE+1};
        
    private:
        Delta<T> *value;
        BASE_TYPE *delta;
        bool has;
        
    public:
        Delta(const BASE_TYPE &increment, const Delta<T> &init): BASE_TYPE(increment), value(new Delta<T>(init)), delta(this), has(true) {};
        explicit Delta(const BASE_TYPE &increment): BASE_TYPE(increment), value(NULL), delta(this), has(false) {};
        Delta(const BASE_TYPE &increment, Delta<T> * const val): BASE_TYPE(increment), value(val), delta(this), has(false) {};
        Delta(Delta<T> * const val): BASE_TYPE(), value(val), delta(this), has(false) {};
        Delta(const Delta<Delta<T> *> &other, bool give=false): BASE_TYPE(other), value(other.value), delta(this), has(give) {};
        Delta(): BASE_TYPE(), value(NULL), delta(this), has(false) {};
        virtual ~Delta() {if(has) delete value;};
        
    public:
        void set(const Delta<T>& val)
        {
            if(this->value == NULL)
            {
                this->value = new Delta<T>(val);
                has = true;
            }
            else
            {
                *(this->value) = val;
            }
        }
        
        void set(Delta<T> * const valPtr)
        {
            if(this->value && has)
            {
                delete this->value;
            }
            
            this->value = valPtr;
            has = false;
        }
        
        
    public:
        const BASE_TYPE &get(int i=1) const {if(i == N_DERIVATIVE) return *value; else return value->get(i);};        
        void increment(const double iterations=1.0) {value += iterations*(*delta); value->increment();};
        const SUCCESS loop() override {increment(); return SUCCEEDED;};
    };
    
}


#endif