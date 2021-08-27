#if !defined(QUETZAL_MATH_DIMENSIONREDUCER_HPP)
#define QUETZAL_MATH_DIMENSIONREDUCER_HPP
//------------------------------------------------------------------------------
// math
// DimensionReducer.hpp
//------------------------------------------------------------------------------

#include "Vector.hpp"
#include <functional>
#include <cmath>

namespace quetzal::math
{

	//--------------------------------------------------------------------------
	template<typename Traits>
	class DimensionReducer
	{
	public:

		using value_type = Traits::value_type;
		using vector_type = Vector<Traits>;
		using vector_reduced_type = Vector<typename Traits::reduced_traits>;

		DimensionReducer() = default;
		DimensionReducer(const vector_type& normal);
		DimensionReducer(const DimensionReducer&) = default;

		DimensionReducer& operator=(const DimensionReducer&) = default;

		void init(const vector_type& normal);

		vector_reduced_type reduce(const vector_type& v) const;

	private:

		std::function<vector_reduced_type(const vector_type& v)> m_reducer;

		static vector_reduced_type reduce_x_yz(const vector_type& v);
		static vector_reduced_type reduce_x_zy(const vector_type& v);
		static vector_reduced_type reduce_y_zx(const vector_type& v);
		static vector_reduced_type reduce_y_xz(const vector_type& v);
		static vector_reduced_type reduce_z_xy(const vector_type& v);
		static vector_reduced_type reduce_z_yx(const vector_type& v);
	};

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::DimensionReducer<Traits>::DimensionReducer(const vector_type& normal) :
	m_reducer()
{
	init(normal);
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::math::DimensionReducer<Traits>::init(const vector_type& normal)
{
    assert(normal.unit());

	if (std::abs(normal.z()) >= std::abs(normal.x()) && std::abs(normal.z()) >= std::abs(normal.y()))
	{
		m_reducer = normal.z() > 0 ? reduce_z_xy : reduce_z_yx;
	}
	else if (std::abs(normal.y()) >= std::abs(normal.x()))
	{
		m_reducer = normal.y() > 0 ? reduce_y_zx : reduce_y_xz;
	}
	else
	{
		m_reducer = normal.x() > 0 ? reduce_x_yz : reduce_x_zy;
	}

	return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce(const vector_type& v) const
{
	return m_reducer(v);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce_x_yz(const vector_type& v)
{
	return {v.y(), v.z()};
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce_x_zy(const vector_type& v)
{
	return {-v.y(), v.z()};
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce_y_zx(const vector_type& v)
{
	return {-v.x(), v.z()};
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce_y_xz(const vector_type& v)
{
	return {v.x(), v.z()};
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce_z_xy(const vector_type& v)
{
	return {v.x(), v.y()};
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::DimensionReducer<Traits>::vector_reduced_type quetzal::math::DimensionReducer<Traits>::reduce_z_yx(const vector_type& v)
{
	return {-v.x(), v.y()};
}

#endif // QUETZAL_MATH_DIMENSIONREDUCER_HPP
