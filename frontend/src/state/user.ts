import { create } from "zustand"

interface UserStore {
    login: string,
    username: string,
    setUser: (login: string, username: string) => void
}

export const useUserStore = create<UserStore>()((set) => ({
    login: "",
    username: "",
    setUser: (login: string, username: string) => set({ login, username })
}))
