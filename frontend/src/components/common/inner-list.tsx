import { useNavigate } from "@solidjs/router";
import { Close as ClearIcon, Edit as EditIcon, Search as SearchIcon } from "@suid/icons-material";
import { Button, Checkbox, Divider, IconButton, Link, List, ListItem, ListItemButton, ListItemIcon, ListItemText, Stack, TextField } from "@suid/material";
import { Accessor, createEffect, createSignal, For, Setter } from "solid-js";
import { serverURL } from "~/data/fetcher";

export type AnyItem = {
  id: number,
  name: string,
}

type InnerListProps<Item extends AnyItem> = {
  path: string,
  choices: Accessor<number[]>,
  setChoices: Setter<number[]>,
  extractText: (item: Item) => string,
}

export default function InnerList<Item extends AnyItem>(props: InnerListProps<Item>) {
  const navigator = useNavigate()

  const [items, setItems] = createSignal<Item[] | undefined>(undefined)
  createEffect(() => {
    fetch(`${serverURL}/${props.path}/`)
      .then(res => res.json())
      .then(data => setItems(data as Item[]))
  })
  const [input, setInput] = createSignal<string>("")

  return <Stack direction="column" sx={{ border: "1px solid #E6E6E6", borderRadius: 3 }}>
    <Stack direction="row" alignItems="center">
      <IconButton disableFocusRipple disableRipple disableTouchRipple>
        <SearchIcon />
      </IconButton>
      <TextField
        fullWidth
        placeholder="Search..."
        value={input()}
        onChange={({ target: { value } }) => setInput(value)}
      />
      <IconButton onClick={() => setInput("")}>
        <ClearIcon />
      </IconButton>
    </Stack>
    <List sx={{
      width: "100%",
      minHeight: "70vh",
      padding: 0,
      borderTop: "1px solid #E6E6E6",
      borderBottom: "1px solid #E6E6E6",
    }}>
      {items() === undefined
        ? <ListItem>
          Loading...
        </ListItem>
        : <For each={items()?.filter(item => item.name.includes(input()))}>
          {(item: Item) => <>
            <ListItem
              disablePadding
              secondaryAction={
                <IconButton>
                  <Link href={`/${props.path}/${item.id}`}>
                    <IconButton>
                      <EditIcon />
                    </IconButton>
                  </Link>
                </IconButton>
              }
            >
              <ListItemButton
                onClick={() => {
                  console.log(props.choices().includes(item.id), props.choices())
                  if (props.choices().includes(item.id)) props.setChoices(props.choices().filter(id => id !== item.id))
                  else props.setChoices(props.choices().concat(item.id))
                  console.log(props.choices().includes(item.id), props.choices())
                }}
              >
                <ListItemIcon>
                  <Checkbox disableRipple checked={props.choices().includes(item.id)} />
                </ListItemIcon>
                <ListItemText>
                  {props.extractText(item)}
                </ListItemText>
              </ListItemButton>
            </ListItem>
            <Divider />
          </>}
        </For>
      }
    </List>
    <Button sx={{ height: "60px" }} onClick={() => navigator(`/${props.path}/new`)}>
      Create
    </Button>
  </Stack>
}
