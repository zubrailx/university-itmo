import { Box, Typography } from "@suid/material";
import { Title } from "solid-start";

export default function Home() {
  return (
    <main>
      <Title>Hello World</Title>
      <Box width="400px" margin="auto">
        <Typography variant="h3">Hello World</Typography>
      </Box>
    </main>
  );
}
