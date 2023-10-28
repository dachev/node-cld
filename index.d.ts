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

export declare function detect(text: string, options: Options, callback: (err: string, result: DetectLanguage) => void): void;
export declare function detect(text: string, callback: (err: string, result: DetectLanguage) => void): void;
export declare function detect(text: string, options: Options): Promise<DetectLanguage>;
export declare function detect(text: string): Promise<DetectLanguage>;
