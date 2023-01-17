import { useNavigate } from "@solidjs/router";
import { Close as ClearIcon, Edit as EditIcon, Search as SearchIcon } from "@suid/icons-material";
import { Button, Checkbox, Dialog, DialogActions, DialogContent, DialogTitle, Divider, IconButton, Link, List, ListItem, ListItemButton, ListItemIcon, ListItemText, Stack, TextField } from "@suid/material";
import { createEffect, createSignal, For, JSXElement } from "solid-js";
import { serverURL } from "~/data/fetcher";
import { getToken } from "~/data/user-store";

export type AnyItem = {
  id: number,
  name: string,
}

export type AnyLink = {
  item: number,
}

export type LinkDialogProps<Item extends AnyItem, Link extends AnyLink> = {
  item?: Item,
  link?: Link,
  editLink: (newData?: Link) => void,
}

export type InnerListProps<Item extends AnyItem, Link extends AnyLink> = {
  path: string,
  extractText: (item: Item) => string,
  linkDialog?: (props: LinkDialogProps<Item, Link>) => JSXElement,
  outerPath?: string,
  outerId?: string,
  width?: string,
}

export default function InnerList<Item extends AnyItem, Link extends AnyLink>(props: InnerListProps<Item, Link>) {
  const navigator = useNavigate()

  const [input, setInput] = createSignal<string>("")

  const [items, setItems] = createSignal<Item[] | undefined>(undefined)
  createEffect(() => {
    fetch(
      `${serverURL}/${props.path}/`,
      { headers: { "authorization": `Bearer ${getToken()}` } },
    )
      .then(res => {
        if (res.ok) res.json().then(data => setItems(data as Item[]))
        else if (res.status === 401) navigator("/")
        else if (res.status === 403) navigator("/supplier")
        else res.text().then(text => console.log(res.status, text))
      })
  })

  const [links, setLinks] = createSignal<Link[] | undefined>(undefined)
  createEffect(() => {
    fetch(
      `${serverURL}/${props.outerPath}/${props.outerId}/items/`,
      { headers: { "authorization": `Bearer ${getToken()}` } },
    )
      .then(res => {
        if (res.ok) res.json().then(data => {
          setLinks(data as Link[]);
          console.log(links())
        })
        else if (res.status === 401) navigator("/")
        else if (res.status === 403) navigator("/supplier")
        else res.text().then(text => console.log(res.status, text))
      })
  })

  function editLink(itemId: number, newData?: Link) {
    fetch(
      `${serverURL}/${props.outerPath}/items`, {
      method: newData === undefined ? "DELETE" : "POST",
      credentials: "include",
      cache: "no-cache",
      headers: { "content-type": "application/json", "authorization": getToken() },
      body: JSON.stringify({
        main: props.outerId,
        item: itemId,
        ...newData || {},
      })
    }).then(res => {
      if (res.ok) {
        if (newData === undefined) setLinks(links()?.filter(link => link.item !== itemId))
        else setLinks(links()?.concat([{ main: props.outerId, ...newData }]))
        console.log(links())
      }
      else res.text().then(text => console.log(res.status, text))
    })
    setOpenLink(undefined)
  }

  const [openLink, setOpenLink] = createSignal<number | undefined>(undefined)

  return <Stack direction="column" sx={{ width: props.width, border: "1px solid #E6E6E6", borderRadius: 3 }}>
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
      height: "70vh",
      overflow: "auto",
      padding: 0,
      borderTop: "1px solid #E6E6E6",
      borderBottom: "1px solid #E6E6E6",
    }}>
      {items() === undefined || links() === undefined
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
              <ListItemButton onClick={() => setOpenLink(item.id)}>
                <ListItemIcon>
                  <Checkbox
                    disableRipple
                    checked={links()?.find(link => link.item === item.id) !== undefined}
                  />
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
    <Dialog
      open={openLink() !== undefined}
      onClose={() => setOpenLink(undefined)}
      PaperProps={{ sx: { width: "400px", padding: "20px" } }}
    >
      {props.linkDialog && props.linkDialog({
        item: items()?.find(item => item.id === openLink()),
        link: links()?.find(link => link.item === openLink()),
        editLink: (data) => editLink(openLink() || -20, data),
      })}
    </Dialog>
  </Stack>
}
