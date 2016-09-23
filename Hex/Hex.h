#ifndef HEX



#define _USE_MATH_DEFINES

#include <assert.h>
#include <vector>
#include <math.h>
#include <SFML\Graphics.hpp>

namespace hex
{
	struct Orientation 
	{
		const double f0, f1, f2, f3;
		const double b0, b1, b2, b3;
		const double start_angle; // in multiples of 60°
		Orientation(double f0_, double f1_, double f2_, double f3_,
			double b0_, double b1_, double b2_, double b3_,
			double start_angle_)
			: f0(f0_), f1(f1_), f2(f2_), f3(f3_),
			b0(b0_), b1(b1_), b2(b2_), b3(b3_),
			start_angle(start_angle_) {}
	};

	const Orientation LAYOUT_POINTY
		= Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
			sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
			0.5);
	const Orientation LAYOUT_FLAT
		= Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0),
			2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0,
			0.0);

	struct Layout
	{
		const Orientation orientation;
		const sf::Vector2f size;
		const sf::Vector2f origin;
		Layout(Orientation orientation_, sf::Vector2f size_, sf::Vector2f origin_)
			: orientation(orientation_), size(size_), origin(origin_) {}
	};

	class FractionalHex
	{
	public:
		const double q, r, s;
		FractionalHex(double _q, double _r, double _s) : q(_q), r(_r), s(_s) {};
	};
	class Hex
	{		
	public:
		const int q, r, s;
		Hex(int _q, int _r, int _s) : q(_q), r(_r), s(_s) {};
		bool operator== (Hex b) const { return q == b.q && r == b.r && s == b.s; };
		bool operator!= (Hex b) const { return !(*this == b); };
		Hex operator+ (Hex b) const { return Hex(q + b.q, r + b.r, s + b.s); };
		Hex operator- (Hex b) const { return Hex(q - b.q, r - b.r, s - b.s); };
		Hex operator* (int k) const { return Hex(q * k, r * k, s * k); };

		int length(Hex h) const { return int((abs(h.q) + abs(h.r) + abs(h.s)) / 2); };
		int distance(Hex b) { return length(*this - b); };
		
		

		std::vector<Hex> range(int range) const
		{
			std::vector<Hex> result;
			for (int dx = -range; dx <= range; dx++)
			{
				for (int dy = -range; dy <= range; dy++)
				{
					for (int dz = -range; dz <= range; dz++)
					{
						if (dx + dy + dz == 0)
							result.push_back((*this) + Hex(dx, dy, dz));
					}
				}
			}
			return result;
		}

		sf::Vector2f& toPixel(Layout layout) const
		{
			const Orientation& M = layout.orientation;
			double x = (M.f0 * q + M.f1 * r) * layout.size.x;
			double y = (M.f2 * q + M.f3 * r) * layout.size.y;
			return sf::Vector2f(x + layout.origin.x, y + layout.origin.y);
		}

		std::vector<sf::Vector2f> polygon_corners(Layout layout) const
		{
			std::vector<sf::Vector2f> corners = {};
			sf::Vector2f center = toPixel(layout);
			for (int i = 0; i < 6; i++) 
			{
				sf::Vector2f offset = corner_offset(layout, i);
				corners.push_back(sf::Vector2f(((center.x + offset.x)), (center.y + offset.y)));
			}
			return corners;
		}
	private:
		
		sf::Vector2f corner_offset(Layout layout, int corner) const
		{
			sf::Vector2f size = sf::Vector2f(layout.size.x * 2, layout.size.y * 2);
			double angle = 2.0 * M_PI * (layout.orientation.start_angle + corner) / 6;
			return sf::Vector2f(size.x * cos(angle), size.y * sin(angle));
		}

	};

	const std::vector<Hex> directions = { Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1) };
	inline Hex direction(int _direction) { return directions[_direction]; };
	inline Hex neighbor(Hex h, int _direction) { return (h + direction(_direction)); };

	inline Hex hex_round(FractionalHex h)
	{
		int q = int(round(h.q));
		int r = int(round(h.r));
		int s = int(round(h.s));
		double q_diff = abs(q - h.q);
		double r_diff = abs(r - h.r);
		double s_diff = abs(s - h.s);
		if (q_diff > r_diff && q_diff > s_diff) {
			q = -r - s;
		}
		else if (r_diff > s_diff) {
			r = -q - s;
		}
		else {
			s = -q - r;
		}
		return Hex(q, r, s);
	}

	
	inline Hex pixel_to_hex(Layout layout, sf::Vector2f p)
	{
		const Orientation& M = layout.orientation;
		sf::Vector2f pt = sf::Vector2f((p.x - layout.origin.x) / layout.size.x, (p.y - layout.origin.y) / layout.size.y);
		double q = (M.b0 * pt.x + M.b1 * pt.y) / 2;
		double r = (M.b2 * pt.x + M.b3 * pt.y) / 2;
		return hex_round(FractionalHex(q, r, -q - r));
	}
}
#endif // !HEX