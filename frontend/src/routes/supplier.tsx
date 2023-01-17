import { userFields } from "~/components/common/user-data";
import FormPage from "~/components/form/page";

type Price = {
  nominal_amount: number,
  nominal_cost: number,
  min_count: number,
  max_count: number,
  ingredient: { name: string },
}


export default function SupplierPage() {
  return <>
    <FormPage
      title="Supplier"
      path="users/me/data"
      customId=""
      formWidth="420px"
      listWidth="580px"
      fields={userFields}
      innerList={{
        path: "prices",
        noChecks: true,
        extractText: (item: Price) => `${item.ingredient.name} x ${item.nominal_amount}`,
      }}
    />
  </>
}
