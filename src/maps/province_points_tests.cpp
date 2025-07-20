#include <external/commonItems/external/googletest/googletest/include/gtest/gtest.h>

#include "src/maps/province_points.h"



namespace maps
{

TEST(MapsProvincepoints, CentermostIsOriginIfNoPoints)
{
   const ProvincePoints province_points(std::set<Point>{});

   constexpr Point kExpectedPoint{.x = 0, .y = 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CenterPointIsAverageLatitude)
{
   const ProvincePoints province_points({
       Point{.x = 0, .y = 1},
       Point{.x = 0, .y = 2},
       Point{.x = 0, .y = 3},
       Point{.x = 0, .y = 4},
   });

   constexpr Point kExpectedPoint{.x = 0, .y = 2};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CenterPointIsAverageLongitude)
{
   const ProvincePoints province_points({
       Point{.x = 1, .y = 0},
       Point{.x = 2, .y = 0},
       Point{.x = 3, .y = 0},
       Point{.x = 4, .y = 0},
   });

   constexpr Point kExpectedPoint{.x = 2, .y = 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CentermostPointWhenCenterNotControlled)
{
   const ProvincePoints province_points({Point{.x = 1, .y = 0}, Point{.x = 3, .y = 0}, Point{.x = 10, .y = 0}});

   constexpr Point kExpectedPoint{.x = 3, .y = 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CentermostIsOriginIfNoPointsWhenAddingPoints)
{
   const ProvincePoints province_points;

   constexpr Point kExpectedPoint{.x = 0, .y = 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CenterPointIsAverageLatitudeWhenAddingPoints)
{
   ProvincePoints province_points;
   province_points.AddPoint(Point{.x = 0, .y = 1});
   province_points.AddPoint(Point{.x = 0, .y = 2});
   province_points.AddPoint(Point{.x = 0, .y = 3});
   province_points.AddPoint(Point{.x = 0, .y = 4});

   constexpr Point kExpectedPoint{.x = 0, .y = 2};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CenterPointIsAverageLongitudeWhenAddingPoints)
{
   ProvincePoints province_points;
   province_points.AddPoint(Point{.x = 1, .y = 0});
   province_points.AddPoint(Point{.x = 2, .y = 0});
   province_points.AddPoint(Point{.x = 3, .y = 0});
   province_points.AddPoint(Point{.x = 4, .y = 0});

   constexpr Point kExpectedPoint{.x = 2, .y = 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}


TEST(MapsProvincepoints, CentermostPointWhenCenterNotControlledWhenAddingPoints)
{
   ProvincePoints province_points;
   province_points.AddPoint(Point{.x = 1, .y = 0});
   province_points.AddPoint(Point{.x = 3, .y = 0});
   province_points.AddPoint(Point{.x = 10, .y = 0});

   constexpr Point kExpectedPoint{.x = 3, .y = 0};
   EXPECT_EQ(province_points.GetCentermostPoint(), kExpectedPoint);
}

}  // namespace maps