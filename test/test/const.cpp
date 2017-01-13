#include <vector>
#include <atomic>

using namespace std;
struct point { int x,y;};

class polygon {
public:
    polygon() : area{-1} {}
    polygon( const polygon& other ) : points{other.points}, area{-1} { }
    polygon( polygon&& other ) 
        : points{move(other.points)}, area{other.area.load()}
        { other.area = -1; }
    polygon& operator=( const polygon& other )
        { points = other.points; area = -1; return *this; }

    polygon& operator=( polygon&& other ) {
        points = move(other.points);
        area = other.area.load();
        other.area = -1;
        return *this;
    }

    void add_point( const point& pt ) { area = -1;
                                       points.push_back(pt); }

    point get_point( int i ) const { return points[i]; }
    //void change_i(const int i) { ++i; } // const.cpp:13:36: error: increment of read-only parameter ‘i’


    int get_num_points() const { return points.size(); }

    double get_area() {
        if( area < 0 )   // if not yet calculated and cached
            calc_area();     // calculate now
        return area;
    }

private:
/*
    void calc_area() {
        area = 0;
        //for( vector<point>::const_iterator i = begin(points); i != end(points); ++i )
	for( auto& pt : points)
            area += 1; 
    }
*/
    // since area is atomic now, use temp varaible to easy the load in case it's multi-thread calc_area
    void calc_area() {
        auto tmp = 0.0;
        //for( vector<point>::const_iterator i = begin(points); i != end(points); ++i )
	for( auto& pt : points)
            tmp += 1; 
	area = tmp;
    }
    vector<point> points;
    mutable atomic<double> area;
    //double        area;
};

/* pass both as const & isn't good as lhs pass by value because ret= lhs; makes a copy */
//polygon operator+( const polygon& lhs, const polygon& rhs ) {
/* Prefer passing a read-only parameter by value if you’re going to make a copy of the parameter anyway, because it enables move from rvalue arguments. 
 * If the caller passes a named polygon object (an lvalue), there’s no difference. Both pass-by-const& followed by an explicit copy and pass-by-value will perform one copy. 
 * If the caller passes a temporary polygon object (an rvalue), the compiler automatically move-constructs lhs from that, which probably makes no difference for a small type like polygon but can be considerably cheaper for many types. 
 */
polygon operator+( polygon lhs, const polygon& rhs ) {
    auto ret = lhs;
    auto last = rhs.get_num_points();
    for( auto i = 0; i < last; ++i ) // concatenate
        ret.add_point( rhs.get_point(i) );
    return ret;
}

void f( const polygon& poly ) {
    const_cast<polygon&>(poly).add_point( {0,0} ); // may give undefined when the referenced object was declared as const
}

//void g( polygon& const poly ) { poly.add_point( {1,1} ); } //‘const’ qualifiers cannot be applied to ‘polygon&’
void g( polygon& poly ) { poly.add_point( {1,1} ); }
void h( polygon* const poly ) { poly->add_point( {2,2} ); }

int main() {
    polygon poly;
    const polygon cpoly;

    f(poly);
    f(cpoly); // The result of the const_cast is undefined if the referenced object was declared as const
    g(poly);
    h(&poly);
}
