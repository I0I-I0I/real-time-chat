import { Button, Dropdown, DropdownItem, Typography } from "@/components/UI"
import { useUserStore } from "@/state/user"
import { IChat } from "@/types"
import cls from "@/utils/cls"
import { useState } from "react"
import { ReactSVG } from "react-svg"
import styles from "./ChatsItem.module.css"

interface ChatsItemProps {
    index: number | null
    className?: string
    data: IChat
    onClick?: (chat: IChat) => void
    onClickRemove?: (chatId: number) => void
}

export const ChatsItem = ({
    index,
    className = "",
    data,
    onClick = () => {},
    onClickRemove,
}: ChatsItemProps) => {
    const [isHover, setIsHover] = useState(false)
    const [isActive, setIsActive] = useState(false)
    const currentUserLogin = useUserStore((state) => state.data?.login)

    return (
        <li
            className={cls(styles.item, className)}
            aria-selected
            aria-label="Chat"
            onMouseEnter={() => {
                if (window.innerWidth < 800) return
                setIsHover(true)
            }}
            onMouseLeave={() => {
                if (window.innerWidth < 800) return
                setIsHover(false)
                setIsActive(false)
            }}
        >
            <Button className={styles.button} onClick={() => onClick(data)}>
                <>
                    <ReactSVG className={styles.icon} src={data.img ? data.img : "/account.svg"} />
                    <div className={styles.body}>
                        {index && (
                            <Typography tag="span" variant="text_tiny" className={styles.index}>
                                {String(index)}
                            </Typography>
                        )}
                        <Typography tag="h2" variant="title-4">
                            {data.type === "friend"
                                ? currentUserLogin !== data.name2
                                    ? data.name2
                                    : data.name
                                : data.name}
                        </Typography>
                        {data.lastMessage?.body && (
                            <Typography tag="span" variant="text_tiny">
                                {data.lastMessage.body}
                            </Typography>
                        )}
                    </div>
                </>
            </Button>
            {onClickRemove && (
                <>
                    <Button
                        className={cls(styles.remove_button, isHover ? styles.active : "")}
                        onClick={() => setIsActive((state) => !state)}
                    >
                        <ReactSVG src="/dots.svg" />
                    </Button>
                    <Dropdown dropdownState={isActive} className={styles.dropdown}>
                        <DropdownItem>
                            <Button
                                onClick={() => onClickRemove(data.id)}
                                className={styles.remove}
                            >
                                <>
                                    <ReactSVG src="/close-x.svg" /> Удалить
                                </>
                            </Button>
                        </DropdownItem>
                    </Dropdown>
                </>
            )}
        </li>
    )
}
