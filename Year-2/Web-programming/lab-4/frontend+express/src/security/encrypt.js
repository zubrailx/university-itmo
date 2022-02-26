import { sha224, sha256 } from "js-sha256";


export function encryptUsingSHA256(string) {
    return sha256(string);
}

export function encryptUsingSHA224(string) {
    return sha224(string);
}
