import { Button, Stack, TextField, Typography } from "@suid/material";
import { createEffect, createSignal } from "solid-js";
import { LinkDialogProps } from "~/components/common/inner-list";
import FormPage from "~/components/form/page";
import { FieldMeta } from "~/components/form/types";


enum FormFieldKeys {
  NAME = "name",
  DESCRIPTION = "description",
}

const fields: FieldMeta<FormFieldKeys>[] = [
  { label: "Name", required: true, key: FormFieldKeys.NAME },
  { label: "Description", required: false, key: FormFieldKeys.DESCRIPTION },
]

type Menu = {
  id: number
  name: string,
  description?: string
}

type Recipe = {
  id: number
  name: string,
  description: string,
  calories: number,
}

type RecipeLink = {
  item: number,
  price: number,
}


function Dialog(props: LinkDialogProps<Recipe, RecipeLink>) {
  const [price, setPrice] = createSignal<string>("")
  const [priceError, setPriceError] = createSignal<boolean>(false)

  createEffect(() => props.link?.price ? setPrice(props.link.price.toString()) : undefined)

  return <Stack direction="column" alignItems="center" spacing={1}>
    <Typography variant="h6">
      Edit Link
    </Typography>
    <TextField
      label="Price"
      variant="outlined"
      fullWidth
      value={price()}
      error={priceError()}
      onChange={({ target: { value } }: any) => {
        setPrice(value.replaceAll(/[^\-0-9]/g, "").substring(0, 10))
        setPriceError(false)
      }}
    />
    <Stack alignItems="center" width="100%" spacing={2} direction="row">
      <Button
        color="error"
        variant="contained"
        fullWidth
        onClick={() => props.editLink()}
      >
        Delete
      </Button>
      <Button
        variant="contained"
        fullWidth
        onClick={() => {
          if (!!!price()) setPriceError(true)
          else props.editLink({ item: props.item?.id || -27, ...props.link, price: parseInt(price()) })
        }}
      >
        Save
      </Button>
    </Stack>
  </Stack>
}

export default function MyCafePage() {
  return <>
    <FormPage
      title="Menu"
      path="menus"
      formWidth="420px"
      listWidth="580px"
      fields={fields}
      innerList={{
        path: "recipes",
        linkDialog: Dialog,
        extractText: (item: Recipe) => item.calories ? `${item.name} [${item.calories} cal]` : item.name,
      }}
    />
  </>
}
