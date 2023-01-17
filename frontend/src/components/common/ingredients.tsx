import { Box, List, ListItem, ListItemButton, Popover, TextField } from "@suid/material";
import { Accessor, createEffect, createSignal, For, Setter } from "solid-js";
import { serverURL } from "~/data/fetcher";

type Ingredient = {
  id: number,
  name: string,
}

export type UnitInputProps = {
  width: string,
  error: boolean,
  clearError: () => void,
  ingredient: Accessor<Ingredient>,
  setIngredient: Setter<Ingredient>
}

export default function IngredientsInput(props: UnitInputProps) {
  const [anchorEl, setAnchorEl] = createSignal<HTMLElement | null>(null)
  const open = () => Boolean(anchorEl())
  const handleClose = () => setAnchorEl(null)

  const [items, setItems] = createSignal<Ingredient[] | undefined>(undefined)
  createEffect(() => {
    fetch(`${serverURL}/ingredients/`)
      .then(res => res.json())
      .then(data => setItems(data))
  })

  const [input, setInput] = createSignal<string>("")
  createEffect(() => setInput(props.ingredient().name))

  return <div>
    <TextField
      label="Ingredient"
      error={props.error}
      fullWidth
      onClick={({ currentTarget }) => setAnchorEl(currentTarget)}
      value={input()}
      onChange={({ target: { value } }) => {
        props.clearError()
        setInput(value)
      }}
    />
    <Popover
      open={open()}
      anchorEl={anchorEl()}
      onClose={handleClose}
      anchorOrigin={{
        vertical: "bottom",
        horizontal: "left",
      }}
    >
      <Box
        sx={{
          width: props.width,
        }}
      >
        <List
          sx={{
            width: "100%",
            maxHeight: "300px",
            overflow: "auto",
          }}
        >
          {items() === undefined
            ? <ListItem>
              Loading...
            </ListItem>
            :
            <For each={items()?.filter(item => item.name.startsWith(input()))}>
              {
                (item: Ingredient) => <>
                  <ListItem disablePadding>
                    <ListItemButton
                      onClick={() => {
                        props.clearError()
                        props.setIngredient(item)
                        setInput(item.name)
                        setAnchorEl(null)
                      }}
                    >
                      {item.name}
                    </ListItemButton>
                  </ListItem>
                </>
              }
            </For>
          }
        </List>
      </Box>
    </Popover>
  </div>
}
