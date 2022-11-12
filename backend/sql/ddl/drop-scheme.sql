drop table if exists
  units,
  suppliers,
  cafes,
  menus,
  recipes,
  ingredients,
  prices,
  orders,
  recipe2ingr,
  stocks,
  cafe2menu,
  menu2recipe
cascade;

drop type order_status;
