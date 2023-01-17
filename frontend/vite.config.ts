import solid from "solid-start/vite";
import suid from "@suid/vite-plugin";
import { defineConfig } from "vite";

export default defineConfig({
  plugins: [suid(), solid({ ssr: false })],  // https://github.com/swordev/suid/issues/139
  build: {
    target: "esnext",
  },
});
