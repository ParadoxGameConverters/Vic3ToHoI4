#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/maps/province_points.h"



namespace maps
{

TEST(MapsProvincepoints, CentermostIsOriginIfNoPoints)
{
   const ProvincePoints province_points(std::set<Point>{});

   constexpr Point expected_point{0, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CenterPointIsAverageLatitude)
{
   const ProvincePoints province_points({Point{0, 1}, Point{0, 2}, Point{0, 3}, Point{0, 4}});

   constexpr Point expected_point{0, 2};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CenterPointIsAverageLongitude)
{
   const ProvincePoints province_points({Point{1, 0}, Point{2, 0}, Point{3, 0}, Point{4, 0}});

   constexpr Point expected_point{2, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CentermostPointWhenCenterNotControlled)
{
   const ProvincePoints province_points({Point{1, 0}, Point{3, 0}, Point{10, 0}});

   constexpr Point expected_point{3, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CentermostIsOriginIfNoPointsWhenAddingPoints)
{
   const ProvincePoints province_points;

   constexpr Point expected_point{0, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CenterPointIsAverageLatitudeWhenAddingPoints)
{
   ProvincePoints province_points;
   province_points.AddPoint(Point{0, 1});
   province_points.AddPoint(Point{0, 2});
   province_points.AddPoint(Point{0, 3});
   province_points.AddPoint(Point{0, 4});

   constexpr Point expected_point{0, 2};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CenterPointIsAverageLongitudeWhenAddingPoints)
{
   ProvincePoints province_points;
   province_points.AddPoint(Point{1, 0});
   province_points.AddPoint(Point{2, 0});
   province_points.AddPoint(Point{3, 0});
   province_points.AddPoint(Point{4, 0});

   constexpr Point expected_point{2, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CentermostPointWhenCenterNotControlledWhenAddingPoints)
{
   ProvincePoints province_points;
   province_points.AddPoint(Point{1, 0});
   province_points.AddPoint(Point{3, 0});
   province_points.AddPoint(Point{10, 0});

   constexpr Point expected_point{3, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}

}  // namespace maps