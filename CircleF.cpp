#include "CiecleF.h"

CircleF::CircleF(const position_type& p0, const position_type& p1, const position_type& p2) noexcept
{
	if (p0 == p1)
	{
		*this = CircleF(p0, p2);
		return;
	}
	else if (p0 == p2 || p1 == p2)
	{
		*this = CircleF(p0, p1);
		return;
	}

	const double a02 = 2 * (p0.x - p2.x);
	const double b02 = 2 * (p0.y - p2.y);
	const double c02 = (p0.y * p0.y - p2.y * p2.y) + (p0.x * p0.x - p2.x * p2.x);
	const double a12 = 2 * (p1.x - p2.x);
	const double b12 = 2 * (p1.y - p2.y);
	const double c12 = (p1.y * p1.y - p2.y * p2.y) + (p1.x * p1.x - p2.x * p2.x);
	const double cy = (a02 * c12 - a12 * c02) / (a02 * b12 - a12 * b02);
	const double cx = std::abs(a02) < std::abs(a12) ? ((c12 - b12 * cy) / a12) : ((c02 - b02 * cy) / a02);
	*this = CircleF(cx, cy, p0.distanceFrom(cx, cy));
}

Ellipse CircleF::stretched(const double _x, const double _y) const noexcept
{
	return Ellipse(center, r + _x, r + _y);
}

Ellipse CircleF::scaled(const double sx, const double sy) const noexcept
{
	return Ellipse(center, r * sx, r * sy);
}

bool CircleF::leftClicked() const
{
	return MouseL.down() && mouseOver();
}

bool CircleF::leftPressed() const
{
	return MouseL.pressed() && mouseOver();
}

bool CircleF::leftReleased() const
{
	return MouseL.up() && mouseOver();
}

bool CircleF::rightClicked() const
{
	return MouseR.down() && mouseOver();
}

bool CircleF::rightPressed() const
{
	return MouseR.pressed() && mouseOver();
}

bool CircleF::rightReleased() const
{
	return MouseR.up() && mouseOver();
}

bool CircleF::mouseOver() const
{
	return Geometry2D::Intersect(Cursor::PosF(), *this);
}

const CircleF& CircleF::draw(const ColorF& color) const
{
	s3d::Circle(*this).draw(color);

	return *this;
}

const CircleF& CircleF::drawFrame(const double thickness, const ColorF& color) const
{
	return drawFrame(thickness * 0.5, thickness * 0.5, color);
}

const CircleF& CircleF::drawFrame(double innerThickness, double outerThickness, const ColorF& color) const
{
	s3d::Circle(*this).drawFrame(innerThickness, outerThickness, color);

	return *this;
}

const CircleF& CircleF::drawFrame(double innerThickness, double outerThickness, const ColorF& innerColor, const ColorF& outerColor) const
{
	s3d::Circle(*this).drawFrame(innerThickness, outerThickness, innerColor, outerColor);

	return *this;
}

const CircleF& CircleF::drawPie(double startAngle, double angle, const ColorF& color) const
{
	s3d::Circle(*this).drawPie(startAngle, angle, color);

	return *this;
}

const CircleF& CircleF::drawArc(double startAngle, double angle, double innerThickness, double outerThickness, const ColorF& color) const
{
	s3d::Circle(*this).drawArc(startAngle, angle, innerThickness, outerThickness, color);

	return *this;
}

const CircleF& CircleF::drawShadow(const Vec2& offset, double blurRadius, double spread, const ColorF& color) const
{
	s3d::Circle(*this).drawShadow(offset, blurRadius, spread, color);

	return *this;
}

TexturedCircle CircleF::operator ()(const Texture& texture) const
{
	return TexturedCircle(texture,
		0.0f, 0.0f, 1.0f, 1.0f,
		*this);
}

TexturedCircle CircleF::operator ()(const TextureRegion& textureRegion) const
{
	return TexturedCircle(textureRegion.texture,
		textureRegion.uvRect,
		*this);
}

Polygon CircleF::asPolygon(const uint32 quality) const
{
	return s3d::Circle(*this).asPolygon(quality);
}

void Formatter(FormatData& formatData, const CircleF& value)
{
	Formatter(formatData, Vec3(value.x, value.y, value.r));
}


using namespace AngelScript;

using ShapeType = CircleF;

static void Construct(const CircleF& circle, ShapeType* self)
{
	new(self) ShapeType(circle);
}

static void ConstructD(float r, ShapeType* self)
{
	new(self) ShapeType(r);
}

static void ConstructDDD(float x, float y, float r, ShapeType* self)
{
	new(self) ShapeType(x, y, r);
}

static void ConstructVD(const Float2& center, float r, ShapeType* self)
{
	new(self) ShapeType(center, r);
}

void RegisterCircleF()
{
	auto engine = ScriptManager::GetEngine();
	constexpr char TypeName[] = "CircleF";

	int32 r = 0;
	r = engine->RegisterObjectProperty(TypeName, "float x", asOFFSET(ShapeType, x)); assert(r >= 0);
	r = engine->RegisterObjectProperty(TypeName, "float y", asOFFSET(ShapeType, y)); assert(r >= 0);
	r = engine->RegisterObjectProperty(TypeName, "float r", asOFFSET(ShapeType, r)); assert(r >= 0);
	r = engine->RegisterObjectProperty(TypeName, "Float2 center", asOFFSET(ShapeType, center)); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(TypeName, asBEHAVE_CONSTRUCT, "void f(const CircleF& in)", asFUNCTION(Construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(TypeName, asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(ConstructD), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(TypeName, asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(ConstructDDD), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(TypeName, asBEHAVE_CONSTRUCT, "void f(const Vec2& in, float)", asFUNCTION(ConstructVD), asCALL_CDECL_OBJLAST); assert(r >= 0);

	//Arg::

	r = engine->RegisterObjectMethod(TypeName, "bool opEquals(const CircleF& in) const", asMETHOD(ShapeType, operator==), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF& set(float, float, float)", asMETHODPR(ShapeType, set, (float, float, float), ShapeType&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF& set(const Vec2& in)", asMETHODPR(ShapeType, set, (const Float2&, float), ShapeType&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF& set(const CircleF& in)", asMETHODPR(ShapeType, set, (const CircleF&), ShapeType&), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF& setCenter(float, float)", asMETHODPR(ShapeType, setCenter, (float, float), ShapeType&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF& setCenter(const Vec2& in)", asMETHODPR(ShapeType, setCenter, (const Float2&), ShapeType&), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF& setPos(float, float)", asMETHODPR(ShapeType, setPos, (float, float), ShapeType&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF& setPos(const Vec2& in)", asMETHODPR(ShapeType, setPos, (const Float2&), ShapeType&), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF& setR(float r)", asMETHOD(ShapeType, setR), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF movedBy(float, float) const", asMETHODPR(ShapeType, movedBy, (float, float) const, ShapeType), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF movedBy(const Vec2& in) const", asMETHODPR(ShapeType, movedBy, (const Float2&) const, ShapeType), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF& moveBy(float, float)", asMETHODPR(ShapeType, moveBy, (float, float), ShapeType&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "CircleF& moveBy(const Vec2 &in)", asMETHODPR(ShapeType, moveBy, (const Float2&), ShapeType&), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF stretched(float) const", asMETHODPR(ShapeType, stretched, (float) const, ShapeType), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "Ellipse stretched(double, double) const", asMETHODPR(ShapeType, stretched, (double, double) const, Ellipse), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "CircleF scaled(double) const", asMETHODPR(ShapeType, scaled, (double) const, ShapeType), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "Ellipse scaled(double, double) const", asMETHODPR(ShapeType, scaled, (double, double) const, Ellipse), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "Vec2 top() const", asMETHOD(ShapeType, top), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "Vec2 right() const", asMETHOD(ShapeType, right), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "Vec2 bbottoml() const", asMETHOD(ShapeType, bottom), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "Vec2 left() const", asMETHOD(ShapeType, left), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "Line lineDiameter() const", asMETHOD(ShapeType, lineDiameter), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "float area() const", asMETHOD(ShapeType, area), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "float perimeter() const", asMETHOD(ShapeType, perimeter), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Point& in) const", asMETHODPR(ShapeType, intersects, (const Point&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Vec2& in) const", asMETHODPR(ShapeType, intersects, (const Vec2&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Line& in) const", asMETHODPR(ShapeType, intersects, (const Line&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Bezier2& in) const", asMETHODPR(ShapeType, intersects, (const Bezier2&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Bezier3& in) const", asMETHODPR(ShapeType, intersects, (const Bezier3&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Rect& in) const", asMETHODPR(ShapeType, intersects, (const Rect&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const RectF& in) const", asMETHODPR(ShapeType, intersects, (const RectF&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const CircleF& in) const", asMETHODPR(ShapeType, intersects, (const CircleF&) const, bool), asCALL_THISCALL); assert(r >= 0);
	//r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Ellipse& in) const", asMETHODPR(ShapeType, intersects, (const Ellipse&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Triangle& in) const", asMETHODPR(ShapeType, intersects, (const Triangle&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Quad& in) const", asMETHODPR(ShapeType, intersects, (const Quad&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const RoundRect& in) const", asMETHODPR(ShapeType, intersects, (const RoundRect&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const Polygon& in) const", asMETHODPR(ShapeType, intersects, (const Polygon&) const, bool), asCALL_THISCALL); assert(r >= 0);
	//r = engine->RegisterObjectMethod(TypeName, "bool intersects(const MultiPolygon& in) const", asMETHODPR(ShapeType, intersects, (const MultiPolygon&) const, bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool intersects(const LineString& in) const", asMETHODPR(ShapeType, intersects, (const LineString&) const, bool), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "bool leftClicked() const", asMETHOD(ShapeType, leftClicked), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool leftPressed() const", asMETHOD(ShapeType, leftPressed), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool leftReleased() const", asMETHOD(ShapeType, leftReleased), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool rightClicked() const", asMETHOD(ShapeType, rightClicked), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool rightPressed() const", asMETHOD(ShapeType, rightPressed), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool rightReleased() const", asMETHOD(ShapeType, rightReleased), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "bool mouseOver() const", asMETHOD(ShapeType, mouseOver), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "const CircleF& draw(const ColorF&in color = Palette::White) const", asMETHOD(ShapeType, draw), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "const CircleF& drawFrame(double thickness = 1.0, const ColorF& in color = Palette::White) const", asMETHODPR(ShapeType, drawFrame, (double, const ColorF&) const, const CircleF&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "const CircleF& drawFrame(double inner, double outer, const ColorF& in color = Palette::White) const", asMETHODPR(ShapeType, drawFrame, (double, double, const ColorF&) const, const CircleF&), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "const CircleF& drawFrame(double inner, double outer, const ColorF& in innerColor, const ColorF& in outerColor) const", asMETHODPR(ShapeType, drawFrame, (double, double, const ColorF&, const ColorF&) const, const CircleF&), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "const CircleF& drawPie(double startAngle, double angle, const ColorF& in color = Palette::White) const", asMETHOD(ShapeType, drawPie), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "const CircleF& drawArc(double startAngle, double angle, double innerThickness = 1.0, double outerThickness = 0.0, const ColorF& in color = Palette::White) const", asMETHOD(ShapeType, drawArc), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "const CircleF& drawShadow(const Vec2& in offset, double blurRadius, double spread = 0.0, const ColorF& in color = ColorF(0.0, 0.5)) const", asMETHOD(ShapeType, drawShadow), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "TexturedCircle opCall(const Texture& in) const", asMETHODPR(ShapeType, operator(), (const Texture&) const, TexturedCircle), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(TypeName, "TexturedCircle opCall(const TextureRegion& in) const", asMETHODPR(ShapeType, operator(), (const TextureRegion&) const, TexturedCircle), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectMethod(TypeName, "Polygon asPolygon(uint32 quality = 24) const", asMETHOD(ShapeType, asPolygon), asCALL_THISCALL); assert(r >= 0);
}