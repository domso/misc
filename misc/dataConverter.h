



template<typename T>
T* objConv(char* data, int n) {
    if (sizeof(T) != n) {
        return 0;
    }
    return (T*)data;
}

template<typename T>
class interval_t {
public:
    interval_t() {
        
    }
    
    interval_t(const T min,const T max) : min_(min), max_(max) {
        
    }
        
    void set_min(const T min) {
        min_ = min;
    }
    
    void set_max(const T max) {
        max_ = max;
    }
    
    bool check(const T comp) const {
        return min_ <= comp && max_ >= comp;
    }
    
    T cut(const T comp) const {
        if (comp < min_) {
           return min_; 
        }
        if (max_ < comp) {
           return max_; 
        }
        return comp;
    }
    
    T next(const T current) const {
        if (max_ <= current) {
            return min_;
        }
    }
    
    T last(const T current) const {
        if (current <= min_) {
            return max_;
        }
    }
    
private:
    T min_;
    T max_;
    
};

template<typename T>
class ring_t {
public:
    ring_t() {
        
    }
    
    ring_t(const T min,const T max) : interval_(min, max), current_(min) {
        
    }
    
    ring_t(const T min,const T max, const T init) : interval_(min, max), current_(init) {
        
    }
    
    T operator++(int) {
        T current = current_;
	current_ = interval_.next(current);
	return current;
    }
    

    T operator--(int) {
        T current = current_;
	current_ = interval_.last(current);
	return current;
        
    }

    
    
    
private:
    T current_;
    interval_t<T> interval_;
    
};

//-----------------------------------------------------------------------------------------------------------------------------------------
struct point2D_t {
    int x;
    int y;
};
//-----------------------------------------------------------------------------------------------------------------------------------------
// A binary-interleaved linear (x,y)-Position
// 
// The (x,y)-Coordinate is stored in a single field-number
// Example:
//          (0110, 0011) -> (0001 1110)
// 
// A linear Iteration through a 2D-matrix would change the pattern from the classial row/column-major sequence to a different approach: 
// 0   1   4   5
// 2   3   6   7 
// 8   9   12  13
// 10  11  14  15
class position_t {
public:
    // Default Constructor shouldnt do anything
    position_t();
    // Sets init-position as field-number
    position_t(const int field);
    // Sets init-position on (x,y)
    position_t(const int x, const int y);
    // Sets init-position on point
    position_t(const point2D_t& point);
   
    
    // Calculates the x-Position   
    int x() const;
    // Calculates the y-Position
    int y() const;
    // Calculates the (x,y)-Point
    point2D_t point() const;
    // Returns the field-number
    int get_field() const;
    
    
    // Sets new position as field-number
    void set(const int field);
    // Sets new position on (x,y)
    void set(const int x, const int y);
    // Sets new position on (x,y)
    void set(const point2D_t& point);
    
    int incX();
    int incY();
    int decX();
    int decY();
private:    
    // field-position
    int field_;
    
};
//-----------------------------------------------------------------------------------------------------------------------------------------
// Constructors for position_t
position_t::position_t() {
    
}

position_t::position_t(const int field) : field_(field) {
    
}

position_t::position_t(const int x, const int y) {
    set(x, y);
}

position_t::position_t(const point2D_t& point) {
    set(point);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// Getter for position_t
int position_t::x() const {
    int x = 0;
    
    // for every bit in field_
    for (int i = 0; i < sizeof(field_)*4; i++) {      
        // get all even bits
        x = x | ((1 << (2 * i)) & field_) >> i;        
    }

    return x;
}

int position_t::y() const {
    int y = 0;
    
    // for every bit in field_
    for (int i = 0; i < sizeof(field_)*4; i++) {
        // get all odd bits
        y = y | ((2 << (2 * i)) & field_) >> (i+1);
    }

    return y;
}

point2D_t position_t::point() const {
    point2D_t result;
    result.x = x();
    result.y = y();
    return result;
    
}

int position_t::get_field() const  {
    return field_;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// Setter for position_t
void position_t::set(const int field) {
    field_ = field;
}

void position_t::set(const int x, const int y) {   
    // for every bit in field_
    field_ = 0;
    for (int i=0; i < sizeof(field_)*8; i++) {
        // store x and y interleaved in field
        field_ = field_ | (x & (1U << i)) << i | (y & (1U << i)) << (i+1);       
    }
    
}

void position_t::set(const point2D_t& point) {
    set(point.x, point.y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------

int position_t_bit_size_of_int = sizeof(int)*8;
int position_t::incX() {
   int result = 1;
    for (int i=0;i < position_t_bit_size_of_int &&((1U << i) & field_) != 0; i+=2) {
        result = result*4 - 1;      
    }
    field_ += result;
    return field_;
}

int position_t::incY() {
   int result = 2;
    for (int i=1;i < position_t_bit_size_of_int && ((1U << i) & field_) != 0; i+=2) {
        result = result*4 - 2;      
    }
    field_ += result;
    return field_;
}

int position_t::decX() {
   int result = 1;
    for (int i=0;i < position_t_bit_size_of_int && ((1U << i) & field_) == 0; i+=2) {
        result = result*4 - 1;      
    }
    field_ += result;
    return result;
}

int position_t::decY() {
   int result = 2;
    for (int i=1;i < position_t_bit_size_of_int && ((1U << i) & field_) == 0; i+=2) {
        result = result*4 - 2;      
    }
    field_ += result;
    return field_;
}


