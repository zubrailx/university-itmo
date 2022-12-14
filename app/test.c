// int test() {
//   // TEST select (passed)
//   {
//     printf("Selection: \n");
//     struct plan_select *select_table1 = plan_select_construct_move(
//         plan_source_construct("table1", dbms), "temp-table1");

//     size_t arr_size;
//     const struct plan_table_info *table_info = select_table1->get_info(select_table1,
//     &arr_size);

//     select_table1->start(select_table1);
//     while (!select_table1->end(select_table1)) {
//       tp_tuple **tpt = select_table1->get(select_table1);
//       print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
//       select_table1->next(select_table1);
//     }
//     select_table1->destruct(select_table1);
//   }

//   // TEST update()
//   {
//     printf("Update: \n");
//     struct column_value *arr = malloc(sizeof(struct column_value) * 2);
//     arr[0].column_name = "column1";
//     arr[0].column_value = "goodbye dfjassdal jf";

//     arr[1].column_name = "column3";
//     arr[1].column_value = "shit";

//     struct plan_update *update_table1 =
//         plan_update_construct_move(plan_source_construct("table1", dbms), 2, arr);

//     size_t arr_size;
//     const struct plan_table_info *table_info = update_table1->get_info(update_table1,
//     &arr_size);

//     update_table1->start(update_table1);
//     while (!update_table1->end(update_table1)) {
//       tp_tuple **tpt = update_table1->get(update_table1);
//       print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
//       update_table1->next(update_table1);
//     }
//     update_table1->destruct(update_table1);
//     free(arr);
//   }

//   // TEST select (passed)
//   {
//     printf("Selection: \n");
//     struct plan_select *select_table1 = plan_select_construct_move(
//         plan_source_construct("table1", dbms), "temp-table1");

//     size_t arr_size;
//     const struct plan_table_info *table_info = select_table1->get_info(select_table1,
//     &arr_size);

//     select_table1->start(select_table1);
//     while (!select_table1->end(select_table1)) {
//       tp_tuple **tpt = select_table1->get(select_table1);
//       print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
//       select_table1->next(select_table1);
//     }
//     select_table1->destruct(select_table1);
//   }

//   // // TEST delete ()
//   {
//     printf("After deletion:\n");
//     struct plan_delete *delete_table1 =
//         plan_delete_construct_move(plan_source_construct("table1", dbms));

//     delete_table1->start(delete_table1);
//     while (!delete_table1->end(delete_table1)) {
//       delete_table1->next(delete_table1);
//     }
//     delete_table1->destruct(delete_table1);
//   }
//   // TEST select (passed)
//   {
//     printf("Selection: \n");
//     struct plan_select *select_table1 = plan_select_construct_move(
//         plan_source_construct("table1", dbms), "temp-table1");

//     size_t arr_size;
//     const struct plan_table_info *table_info = select_table1->get_info(select_table1,
//     &arr_size);

//     select_table1->start(select_table1);
//     while (!select_table1->end(select_table1)) {
//       tp_tuple **tpt = select_table1->get(select_table1);
//       print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
//       select_table1->next(select_table1);
//     }
//     select_table1->destruct(select_table1);
//   }
//   // TEST update()
//   {
//     printf("Update: \n");
//     struct column_value *arr = malloc(sizeof(struct column_value) * 2);
//     arr[0].column_name = "column1";
//     arr[0].column_value = "goodbye dfjassdal jf";

//     arr[1].column_name = "column3";
//     arr[1].column_value = "shit";

//     struct plan_update *update_table1 =
//         plan_update_construct_move(plan_source_construct("table1", dbms), 2, arr);

//     size_t arr_size;
//     const struct plan_table_info *table_info = update_table1->get_info(update_table1,
//     &arr_size);

//     update_table1->start(update_table1);
//     while (!update_table1->end(update_table1)) {
//       tp_tuple **tpt = update_table1->get(update_table1);
//       print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
//       update_table1->next(update_table1);
//     }
//     update_table1->destruct(update_table1);
//     free(arr);
//   }
//   {
//     // TEST delete ()
//     printf("After deletion:\n");
//     struct plan_delete *delete_table1 =
//         plan_delete_construct_move(plan_source_construct("table1", dbms));

//     delete_table1->start(delete_table1);
//     while (!delete_table1->end(delete_table1)) {
//       delete_table1->next(delete_table1);
//     }
//     delete_table1->destruct(delete_table1);
//   }
//   return 0;
// }
