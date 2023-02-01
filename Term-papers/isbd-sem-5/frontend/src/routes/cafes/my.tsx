import { userFields } from "~/components/common/user-data";
import FormPage from "~/components/form/page";

type Menu = {
  name: string,
  description?: string,
}

export default function MyCafePage() {
  return <>
    <FormPage
      editOnly
      title="Cafe"
      path="users/me/data"
      customId=""
      formWidth="420px"
      listWidth="580px"
      fields={userFields}
      innerList={{
        path: "menus",
        noChecks: true,
        extractText: (item: Menu) => item.name,
      }}
    />
  </>
}
