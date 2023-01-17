import { Box, List, ListItem, ListItemButton, Popover, TextField } from "@suid/material";
import { Accessor, Setter, createSignal, createEffect, For } from "solid-js";
import { serverURL } from "~/data/fetcher";

export type UnitInputProps = {
  width: string,
  error: boolean,
  clearError: () => void,
  unit: Accessor<string>,
  setUnit: Setter<string>
}

export default function UnitInput(props: UnitInputProps) {
  const [anchorEl, setAnchorEl] = createSignal<HTMLElement | null>(null)
  const open = () => Boolean(anchorEl())
  const handleClose = () => setAnchorEl(null)

  const [units, setUnits] = createSignal<string[] | undefined>(undefined)
  createEffect(() => {
    fetch(`${serverURL}/units/`)
      .then(res => res.json())
      .then(data => setUnits(data.map(({ name }: { name: string }) => name)))
  })

  return <div>
    <TextField
      label="Unit"
      error={props.error}
      fullWidth
      onClick={({ currentTarget }) => setAnchorEl(currentTarget)}
      value={props.unit()}
      onChange={({ target: { value } }) => {
        props.clearError()
        props.setUnit(() => value)
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
          {units() === undefined
            ? <ListItem>
              Loading...
            </ListItem>
            :
            <For each={units()?.filter(item => item.startsWith(props.unit()))}>
              {
                (item: string) => <>
                  <ListItem disablePadding>
                    <ListItemButton
                      onClick={() => {
                        props.clearError()
                        props.setUnit(() => item)
                        setAnchorEl(null)
                      }}
                    >
                      {item}
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