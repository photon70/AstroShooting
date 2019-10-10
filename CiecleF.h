#pragma once

#include <Siv3D.hpp>

struct CircleF
{
	using position_type = Float2;

	using size_type = position_type::value_type;

	using value_type = position_type::value_type;

	S3D_DISABLE_MSVC_WARNINGS_PUSH(4201)

		union
	{
		/// <summary>
		/// ‰~‚Ì’†SÀ•W
		/// </summary>
		position_type center;

		struct
		{
			/// <summary>
			/// ‰~‚Ì’†S‚Ì X À•W
			/// </summary>
			value_type x;

			/// <summary>
			/// ‰~‚Ì’†S‚Ì Y À•W
			/// </summary>
			value_type y;
		};
	};

	/// <summary>
	/// ‰~‚Ì”¼Œa
	/// </summary>
	size_type r;

	S3D_DISABLE_MSVC_WARNINGS_POP()

		CircleF() = default;

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	explicit constexpr CircleF(size_type _r) noexcept
		: center(0.f, 0.f)
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="_x">
	/// ‰~‚Ì’†S‚Ì X À•W
	/// </param>
	/// <param name="_y">
	/// ‰~‚Ì’†S‚Ì Y À•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(value_type _x, value_type _y, size_type _r) noexcept
		: center(_x, _y)
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="_center">
	/// ‰~‚Ì’†S‚ÌÀ•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(const position_type& _center, size_type _r) noexcept
		: center(_center)
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="_center">
	/// ‰~‚Ì’†S‚ÌÀ•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(Arg::center_<position_type> _center, size_type _r) noexcept
		: center(_center.value())
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="topLeft">
	/// ‰~‚ÉŠOÚ‚·‚é³•ûŒ`‚Ì¶ã‚ÌÀ•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(Arg::topLeft_<position_type> topLeft, size_type _r) noexcept
		: center(topLeft->x + _r, topLeft->y + _r)
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="topRight">
	/// ‰~‚ÉŠOÚ‚·‚é³•ûŒ`‚Ì‰Eã‚ÌÀ•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(Arg::topRight_<position_type> topRight, size_type _r) noexcept
		: center(topRight->x - _r, topRight->y + _r)
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="bottomLeft">
	/// ‰~‚ÉŠOÚ‚·‚é³•ûŒ`‚Ì¶‰º‚ÌÀ•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(Arg::bottomLeft_<position_type> bottomLeft, size_type _r) noexcept
		: center(bottomLeft->x + _r, bottomLeft->y - _r)
		, r(_r) {}

	/// <summary>
	/// ‰~‚ğì¬‚µ‚Ü‚·B
	/// </summary>
	/// <param name="bottomRight">
	/// ‰~‚ÉŠOÚ‚·‚é³•ûŒ`‚Ì‰E‰º‚ÌÀ•W
	/// </param>
	/// <param name="_r">
	/// ‰~‚Ì”¼Œa
	/// </param>
	constexpr CircleF(Arg::bottomRight_<position_type> bottomRight, size_type _r) noexcept
		: center(bottomRight->x - _r, bottomRight->y - _r)
		, r(_r) {}

	constexpr CircleF(Arg::topCenter_<position_type> topCenter, size_type _r) noexcept
		: center(topCenter->x, topCenter->y + _r)
		, r(_r) {}

	constexpr CircleF(Arg::bottomCenter_<position_type> bottomCenter, size_type _r) noexcept
		: center(bottomCenter->x, bottomCenter->y - _r)
		, r(_r) {}

	constexpr CircleF(Arg::leftCenter_<position_type> leftCenter, size_type _r) noexcept
		: center(leftCenter->x + _r, leftCenter->y)
		, r(_r) {}

	constexpr CircleF(Arg::rightCenter_<position_type> rightCenter, size_type _r) noexcept
		: center(rightCenter->x - _r, rightCenter->y)
		, r(_r) {}

	CircleF(const position_type& p0, const position_type& p1) noexcept
		: center((p0 + p1) / 2.f)
		, r(p0.distanceFrom(p1) / 2.f) {}

	CircleF(const position_type& p0, const position_type& p1, const position_type& p2) noexcept;

	explicit CircleF(const Line& diameter) noexcept
		: CircleF(diameter.begin, diameter.end) {}

	CircleF(Arg::center_<position_type> _center, const position_type& p) noexcept
		: center(_center.value())
		, r(p.distanceFrom(_center.value())) {}

	[[nodiscard]] constexpr bool operator ==(const CircleF& circle) const noexcept
	{
		return center == circle.center
			&& r == circle.r;
	}

	[[nodiscard]] constexpr bool operator !=(const CircleF& circle) const noexcept
	{
		return !(*this == circle);
	}

	constexpr CircleF& set(value_type _x, value_type _y, size_type _r) noexcept
	{
		center.set(_x, _y);
		r = _r;
		return *this;
	}

	constexpr CircleF& set(const position_type& _center, size_type _r) noexcept
	{
		return set(_center.x, _center.y, _r);
	}

	constexpr CircleF& set(Arg::center_<position_type> _center, size_type _r) noexcept
	{
		return set(_center->x, _center->y, _r);
	}

	constexpr CircleF& set(Arg::topLeft_<position_type> topLeft, size_type _r) noexcept
	{
		return set(topLeft->x + _r, topLeft->y + _r, _r);
	}

	constexpr CircleF& set(Arg::topRight_<position_type> topRight, size_type _r) noexcept
	{
		return set(topRight->x - _r, topRight->y + _r, _r);
	}

	constexpr CircleF& set(Arg::bottomLeft_<position_type> bottomLeft, size_type _r) noexcept
	{
		return set(bottomLeft->x + _r, bottomLeft->y - _r, _r);
	}

	constexpr CircleF& set(Arg::bottomRight_<position_type> bottomRight, size_type _r) noexcept
	{
		return set(bottomRight->x - _r, bottomRight->y - _r, _r);
	}

	constexpr CircleF& set(Arg::topCenter_<position_type> topCenter, size_type _r) noexcept
	{
		return set(topCenter->x, topCenter->y + _r, _r);
	}

	constexpr CircleF& set(Arg::bottomCenter_<position_type> bottomCenter, size_type _r) noexcept
	{
		return set(bottomCenter->x, bottomCenter->y - _r, _r);
	}

	constexpr CircleF& set(Arg::leftCenter_<position_type> leftCenter, size_type _r) noexcept
	{
		return set(leftCenter->x + _r, leftCenter->y, _r);
	}

	constexpr CircleF& set(Arg::rightCenter_<position_type> rightCenter, size_type _r) noexcept
	{
		return set(rightCenter->x - _r, rightCenter->y, _r);
	}

	constexpr CircleF& set(const CircleF& circle) noexcept
	{
		return *this = circle;
	}

	constexpr CircleF& setCenter(value_type _x, value_type _y) noexcept
	{
		center.set(_x, _y);
		return *this;
	}

	constexpr CircleF& setCenter(const position_type& _center) noexcept
	{
		return setCenter(_center.x, _center.y);
	}

	constexpr CircleF& setPos(value_type _x, value_type _y) noexcept
	{
		return setCenter(_x, _y);
	}

	constexpr CircleF& setPos(const position_type& _center) noexcept
	{
		return setCenter(_center.x, _center.y);
	}

	constexpr CircleF& setPos(Arg::center_<position_type> _center) noexcept
	{
		return setCenter(_center->x, _center->y);
	}

	constexpr CircleF& setPos(Arg::topLeft_<position_type> topLeft) noexcept
	{
		return setCenter(topLeft->x + r, topLeft->y + r);
	}

	constexpr CircleF& setPos(Arg::topRight_<position_type> topRight) noexcept
	{
		return setCenter(topRight->x - r, topRight->y + r);
	}

	constexpr CircleF& setPos(Arg::bottomLeft_<position_type> bottomLeft) noexcept
	{
		return setCenter(bottomLeft->x + r, bottomLeft->y - r);
	}

	constexpr CircleF& setPos(Arg::bottomRight_<position_type> bottomRight) noexcept
	{
		return setCenter(bottomRight->x - r, bottomRight->y - r);
	}

	constexpr CircleF& setPos(Arg::topCenter_<position_type> topCenter) noexcept
	{
		return setCenter(topCenter->x, topCenter->y + r);
	}

	constexpr CircleF& setPos(Arg::bottomCenter_<position_type> bottomCenter) noexcept
	{
		return setCenter(bottomCenter->x, bottomCenter->y - r);
	}

	constexpr CircleF& setPos(Arg::leftCenter_<position_type> leftCenter) noexcept
	{
		return setCenter(leftCenter->x + r, leftCenter->y);
	}

	constexpr CircleF& setPos(Arg::rightCenter_<position_type> rightCenter) noexcept
	{
		return setCenter(rightCenter->x - r, rightCenter->y);
	}

	constexpr CircleF& setR(float _r) noexcept
	{
		r = _r;
		return *this;
	}

	[[nodiscard]] constexpr CircleF movedBy(value_type _x, value_type _y) const noexcept
	{
		return{ center.movedBy(_x, _y), r };
	}

	[[nodiscard]] constexpr CircleF movedBy(const position_type& v) const noexcept
	{
		return movedBy(v.x, v.y);
	}

	constexpr CircleF& moveBy(value_type _x, value_type _y) noexcept
	{
		center.moveBy(_x, _y);
		return *this;
	}

	constexpr CircleF& moveBy(const position_type& v) noexcept
	{
		return moveBy(v.x, v.y);
	}

	[[nodiscard]] constexpr CircleF stretched(value_type size) const noexcept
	{
		return CircleF(center, r + size);
	}

	[[nodiscard]] Ellipse stretched(double _x, double _y) const noexcept;

	[[nodiscard]] constexpr CircleF scaled(double s) const noexcept
	{
		return CircleF(center, r * s);
	}

	[[nodiscard]] Ellipse scaled(double sx, double sy) const noexcept;

	[[nodiscard]] constexpr position_type top() const noexcept
	{
		return{ center.x, center.y - r };
	}

	[[nodiscard]] constexpr position_type right() const noexcept
	{
		return{ center.x + r, center.y };
	}

	[[nodiscard]] constexpr position_type bottom() const noexcept
	{
		return{ center.x, center.y + r };
	}

	[[nodiscard]] constexpr position_type left() const noexcept
	{
		return{ center.x - r, center.y };
	}

	[[nodiscard]] constexpr Line lineDiameter() const noexcept
	{
		return{ left(), right() };
	}

	[[nodiscard]] constexpr value_type area() const noexcept
	{
		return r * r * Math::PiF;
	}

	[[nodiscard]] constexpr value_type perimeter() const noexcept
	{
		return 2 * r * Math::PiF;
	}

	template <class Shape2DType>
	[[nodiscard]] bool intersects(const Shape2DType& shape) const
	{
		return Geometry2D::Intersect(*this, shape);
	}

	template <class Shape2DType>
	[[nodiscard]] Optional<Array<Vec2>> intersectsAt(const Shape2DType& shape) const
	{
		return Geometry2D::IntersectAt(*this, shape);
	}

	template <class Shape2DType>
	[[nodiscard]] bool contains(const Shape2DType& shape) const
	{
		return Geometry2D::Contains(*this, shape);
	}

	[[nodiscard]] bool leftClicked() const;

	[[nodiscard]] bool leftPressed() const;

	[[nodiscard]] bool leftReleased() const;

	[[nodiscard]] bool rightClicked() const;

	[[nodiscard]] bool rightPressed() const;

	[[nodiscard]] bool rightReleased() const;

	[[nodiscard]] bool mouseOver() const;

	const CircleF& paint(Image& dst, const Color& color, bool antialiased = true) const;

	const CircleF& overwrite(Image& dst, const Color& color, bool antialiased = true) const;

	const CircleF& paintFrame(Image& dst, double innerThickness, double outerThickness, const Color& color, bool antialiased = true) const;

	const CircleF& overwriteFrame(Image& dst, double innerThickness, double outerThickness, const Color& color, bool antialiased = true) const;

	//const CircleF& paintPie(Image& dst, double startAngle, double angle, const Color& color) const;

	//const CircleF& overwritePie(Image& dst, double startAngle, double angle, const Color& color) const;

	//const CircleF& paintArc(Image& dst, double startAngle, double angle, double innerThickness, double outerThickness, const Color& color) const;

	//const CircleF& overwriteArc(Image& dst, double startAngle, double angle, double innerThickness, double outerThickness, const Color& color) const;


	/// <summary>
	/// ‰~‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="color">
	/// F
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& draw(const ColorF& color = Palette::White) const;

	/// <summary>
	/// ‰~‚Ì˜g‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="thickness">
	/// ˜g‚Ì‘¾‚³
	/// </param>
	/// <param name="color">
	/// F
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& drawFrame(double thickness = 1.f, const ColorF & color = Palette::White) const;

	/// <summary>
	/// ‰~‚Ì˜g‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="innerThickness">
	/// “à‘¤‚Ì‘¾‚³
	/// </param>
	/// <param name="outerThickness">
	/// ŠO‘¤‚Ì‘¾‚³
	/// </param>
	/// <param name="color">
	/// F
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& drawFrame(double innerThickness, double outerThickness, const ColorF& color = Palette::White) const;

	/// <summary>
	/// ‰~‚Ì˜g‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="innerThickness">
	/// “à‘¤‚Ì‘¾‚³
	/// </param>
	/// <param name="outerThickness">
	/// ŠO‘¤‚Ì‘¾‚³
	/// </param>
	/// <param name="innerColor">
	/// F
	/// </param>
	/// <param name="outerColor">
	/// F
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& drawFrame(double innerThickness, double outerThickness, const ColorF& innerColor, const ColorF& outerColor) const;

	/// <summary>
	/// îŒ`‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="startAngle">
	/// ŠJnŠp“x
	/// </param>
	/// <param name="angle">
	/// Šp“x
	/// </param>
	/// <param name="color">
	/// F
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& drawPie(double startAngle, double angle, const ColorF& color = Palette::White) const;

	/// <summary>
	/// ŒÊ‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="startAngle">
	/// ŠJnŠp“x
	/// </param>
	/// <param name="angle">
	/// Šp“x
	/// </param>
	/// <param name="innerThickness">
	/// “à‘¤‚Ì‘¾‚³
	/// </param>
	/// <param name="outerThickness">
	/// ŠO‘¤‚Ì‘¾‚³
	/// </param>
	/// <param name="color">
	/// F
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& drawArc(double startAngle, double angle, double innerThickness = 1.f, double outerThickness = 0.f, const ColorF & color = Palette::White) const;

	/// <summary>
	/// ‰~‚Ì‰e‚ğ•`‚«‚Ü‚·B
	/// </summary>
	/// <param name="offset">
	/// ‰e‚ÌˆÚ“®—ÊiƒsƒNƒZƒ‹j
	/// </param>
	/// <param name="blurRadius">
	/// ‰e‚Ì‚Ú‚©‚µ‚Ì‘å‚«‚³iƒsƒNƒZƒ‹j
	/// </param>
	/// <param name="spread">
	/// ’·•ûŒ`‚ÌL‚ª‚èiƒsƒNƒZƒ‹j
	/// </param>
	/// <param name="color">
	/// ‰e‚ÌF
	/// </param>
	/// <returns>
	/// *this
	/// </returns>
	const CircleF& drawShadow(const Vec2& offset, double blurRadius, double spread = 0.f, const ColorF & color = ColorF(0.f, 0.5)) const;

	[[nodiscard]] TexturedCircle operator ()(const Texture& texture) const;

	[[nodiscard]] TexturedCircle operator ()(const TextureRegion& textureRegion) const;

	[[nodiscard]] Polygon asPolygon(uint32 quality = 24) const;

	operator s3d::Circle()const {
		return s3d::Circle(center, r);
	}
};