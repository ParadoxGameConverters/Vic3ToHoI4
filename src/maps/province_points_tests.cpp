#include "external/googletest/googletest/include/gtest/gtest.h"
#include "src/maps/province_points.h"



TEST(MapsProvincepoints, CentermostIsOriginIfNoPoints)
{
   const maps::ProvincePoints province_points;

   constexpr maps::Point expected_point{0, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CenterPointIsAverageLatitude)
{
   maps::ProvincePoints province_points;
   province_points.AddPoint(maps::Point{0, 1});
   province_points.AddPoint(maps::Point{0, 2});
   province_points.AddPoint(maps::Point{0, 3});
   province_points.AddPoint(maps::Point{0, 4});

   constexpr maps::Point expected_point{0, 2};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CenterPointIsAverageLongitude)
{
   maps::ProvincePoints province_points;
   province_points.AddPoint(maps::Point{1, 0});
   province_points.AddPoint(maps::Point{2, 0});
   province_points.AddPoint(maps::Point{3, 0});
   province_points.AddPoint(maps::Point{4, 0});

   constexpr maps::Point expected_point{2, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}


TEST(MapsProvincepoints, CentermostPointWhenCenterNotControlled)
{
   maps::ProvincePoints province_points;
   province_points.AddPoint(maps::Point{1, 0});
   province_points.AddPoint(maps::Point{3, 0});
   province_points.AddPoint(maps::Point{10, 0});

   constexpr maps::Point expected_point{3, 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), expected_point);
}