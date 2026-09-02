interface Language {
  readonly name: string;
  readonly code: string;
  readonly percent: number;
  readonly score: number;
}
interface Chunk {
  readonly name: string;
  readonly code: string;
  readonly offset: number;
  readonly bytes: number;
}
interface Options {
  readonly isHTML?: boolean;
  readonly languageHint?: string;
  readonly encodingHint?: string;
  readonly tldHint?: string;
  readonly httpHint?: string;
  readonly bestEffort?: boolean;
}
interface DetectLanguage {
  readonly reliable: boolean;
  readonly textBytes: number;
  readonly languages: Language[];
  readonly chunks: Chunk[];
}

export const LANGUAGES: {[name: string]: string};
export const DETECTED_LANGUAGES: string[];
export const ENCODINGS: string[];

// Browser entry point only (./wasm/browser-entry.mjs). Overrides where the
// WASM backend fetches cld.web.wasm from -- has no effect on the native
// backend. Must be called before the first detect() call.
export declare function setWasmModuleOptions(options: { locateFile?: (path: string, scriptDirectory: string) => string }): void;

export declare function detect(text: string, options: Options, callback: (err: string, result: DetectLanguage) => void): void;
export declare function detect(text: string, callback: (err: string, result: DetectLanguage) => void): void;
export declare function detect(text: string, options: Options): Promise<DetectLanguage>;
export declare function detect(text: string): Promise<DetectLanguage>;
